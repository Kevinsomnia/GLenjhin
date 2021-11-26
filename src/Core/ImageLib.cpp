#include "ImageLib.h"

namespace ImageLib
{
    PNG::Metadata::Metadata() : width(0), height(0), bitDepth(0), colorType(PNG::ColorType::Grayscale), interlaced(false) { }

    PNG::CanonicalHuffman::CanonicalHuffman(const uint16_t* codeLens, size_t codeLensCount)
    {
        m_SymbolCodes = new uint16_t[codeLensCount];
        m_SymbolValues = new uint16_t[codeLensCount];
        m_SymbolCount = 0;

        // Parse code lengths into symbols sorted by ascending code length.
        uint16_t currCode = 0;

        for (uint8_t len = 1; len <= MAX_HUFFMAN_CODE_LENGTH; len++)
        {
            uint16_t bufferBit = 1 << len;  // To differentiate between two codes of different length, append an extra 1 at the most significant bit.
            currCode <<= 1;

            for (uint16_t i = 0; i < codeLensCount; i++)
            {
                if (codeLens[i] == len)
                {
                    m_SymbolCodes[m_SymbolCount] = bufferBit | currCode;
                    m_SymbolValues[m_SymbolCount] = i;
                    m_SymbolCount++;
                    currCode++;
                }
            }
        }
    }

    PNG::CanonicalHuffman::~CanonicalHuffman()
    {
        delete[] m_SymbolCodes;
        delete[] m_SymbolValues;
    }

    uint16_t PNG::CanonicalHuffman::readNextSymbol(MemoryStream& stream) const
    {
        // Finds the first sequence in the stream that matches a symbol in our table.
        // Start at 1 to account for buffer bit.
        uint16_t targetCode = 1;
        uint16_t* codesStart = m_SymbolCodes;
        uint16_t* codesEnd = m_SymbolCodes + m_SymbolCount;

        for (uint8_t i = 0; i < MAX_HUFFMAN_CODE_LENGTH; i++)
        {
            targetCode <<= 1;
            targetCode |= stream.readUInt8(1);

            // Use binary search since the symbols in the array are already sorted in ascending order.
            uint16_t* matchPtr = Algorithms::BinarySearch(codesStart, codesEnd, targetCode);

            if (matchPtr != codesEnd)
                return m_SymbolValues[matchPtr - codesStart];
        }

        cerr << "PNG error: Failed to read next symbol in canonical Huffman" << endl;
        return UINT16_MAX;
    }

    // Invalid/empty result
    PNG::Result::Result() : info(Metadata()), pixels(nullptr) { }

    PNG::Result PNG::Load(const std::string& filePath)
    {
        clock_t benchStart = clock();
        std::ifstream file(filePath, std::ios::binary | std::ios::in);

        if (!file)
        {
            cerr << "Failed to open " << filePath << endl;
            return Result();
        }

        file.seekg(0, file.end);
        size_t fileSize = static_cast<size_t>(file.tellg());
        file.seekg(0, file.beg);
        std::unique_ptr<char[]> fileData(new char[fileSize]);
        file.read(fileData.get(), fileSize);

        MemoryStream stream((uint8_t*)fileData.get(), fileSize);

        // Also verifies IHDR block is the very first block to be parsed.
        if (stream.size() < PNG_FILE_HEADER_INCL_DATA_SIZE || memcmp(stream, PNG_FILE_HEADER, PNG_FILE_HEADER_SIZE) != 0)
        {
            cerr << "PNG file is corrupt: invalid PNG header" << endl;
            return Result();
        }

        stream.advance(PNG_FILE_HEADER_SIZE);
        uint32_t headerCRC = Algorithms::ComputeCRC32(stream - 4, PNG_FILE_HEADER_INCL_DATA_SIZE - PNG_FILE_HEADER_SIZE);

        Metadata meta = Metadata();
        meta.width = stream.readUInt32();
        meta.height = stream.readUInt32();
        meta.bitDepth = stream.readUInt8();
        meta.colorType = static_cast<ColorType>(stream.readUInt8());
        stream.advance(2);  // Skip next 2 bytes which contain compression and filter method (both should be constant).
        meta.interlaced = stream.readUInt8() == 1;

        if (headerCRC != stream.readUInt32())
        {
            cerr << "PNG file is corrupt: IHDR CRC validation failed!" << endl;
            return Result();
        }

        if (!ValidateParameters(meta))
        {
            cerr << "PNG file is corrupt: parameter value(s) are invalid" << endl;
            return Result();
        }

        cout << filePath << endl;
        cout << "WxHxD = " << meta.width << "x" << meta.height << "x" << +meta.bitDepth;
        cout << "  Color type: " << static_cast<uint16_t>(meta.colorType);
        cout << "  Interlaced: " << meta.interlaced << endl;

        // Parse other blocks.
        bool reachedEnd = false;
        bool hasData = false;
        size_t dataLength = 0;

        while (stream.hasData())
        {
            uint32_t chunkLength = stream.readUInt32();
            size_t crcComputeStart = stream.byteOffset();
            uint32_t chunkType = stream.readUInt32();
            size_t dataStreamOffset = 0;

            switch (chunkType)
            {
                case CHUNK_PLTE:
                    if (hasData)
                    {
                        cerr << "PNG file is corrupt: PLTE should appear before IDAT chunk" << endl;
                        return Result();
                    }

                    cout << "found plte" << endl;
                    stream.advance(chunkLength);
                    break;

                case CHUNK_IDAT:
                    // We will write data to the beginning of this stream after the CRC check, so let's get the byte offset to start of data.
                    dataStreamOffset = stream.byteOffset();
                    stream.advance(chunkLength);
                    hasData = true;
                    break;

                case CHUNK_IEND:
                    reachedEnd = true;
                    break;

                case CHUNK_tRNS:
                    // Transparency in indexed color
                    cout << "has tRNS" << endl;
                    stream.advance(chunkLength);
                    break;

                // Ignored ancillary chunks.
                case CHUNK_bKGD:
                case CHUNK_cHRM:
                case CHUNK_gAMA:
                case CHUNK_hIST:
                case CHUNK_iCCP:
                case CHUNK_iEXt:
                case CHUNK_iTXt:
                case CHUNK_pHYs:
                case CHUNK_sBIT:
                case CHUNK_sPLT:
                case CHUNK_sRGB:
                case CHUNK_tEXt:
                case CHUNK_tIME:
                case CHUNK_zTXt:
                    stream.advance(chunkLength);
                    break;

                default:
                    char* typeStr = reinterpret_cast<char*>(&chunkType);
                    cout << "Unknown chunk type: ";
                    for (size_t i = 0; i < 4; i++)
                        cout << stream[i - 4];
                    cout << " (" << chunkType << ")" << endl;
                    stream.advance(chunkLength);
                    break;
            }

            if (reachedEnd)
                break;

            // Validate chunk data.
            size_t crcComputeEnd = stream.byteOffset();
            uint32_t targetCRC = stream.readUInt32();
            stream.setPosition(crcComputeStart);

            // Compute CRC of data block + chunkType (excluding chunkLength) and validate value.
            if (Algorithms::ComputeCRC32(stream, chunkLength + 4) != targetCRC)
            {
                cerr << "PNG file is corrupt: CRC validation failed!" << endl;
                return Result();
            }

            if (dataStreamOffset != 0)
            {
                // This is a hack for performance, similar to: stream.read(stream.begin() + dataLength, chunkLength) but without any extra checks.
                // We can do this because of three things:
                // 1. Since the stream currently contains the binary contents of the entire file, this guarantees that the stream's buffer 
                //    has enough bytes allocated to also store all of the data chunks. We don't have to worry about expanding the capacity at all.
                //    and therefore, all we have to do is copy it to the specified address.
                // 2. We know the IDAT chunk data are byte-aligned. We do not need bit-level support.
                // 3. Once we're done reading a chunk, we don't have to move the byte pointer backwards. This allows us to overwrite the data that we just finished reading.
                uint8_t* streamStartPtr = stream.begin();
                memcpy(streamStartPtr + dataLength, streamStartPtr + dataStreamOffset, chunkLength);
                dataLength += chunkLength;
            }

            // Set position past targetCRC for next chunk.
            stream.setPosition(crcComputeEnd + 4);
        }

        clock_t benchEnd = clock();
        cout << (static_cast<int64_t>(benchEnd) - benchStart) / 1000.0 << " seconds (PROCESS BLOCKS)" << endl;

        if (!reachedEnd)
        {
            cerr << "PNG file is corrupted: file unexpectedly ended before reading IEND chunk" << endl;
            return Result();
        }

        if (!hasData)
        {
            cerr << "PNG file is corrupted: no IDAT chunk was found" << endl;
            return Result();
        }

        benchStart = clock();

        uint32_t pixelCount = meta.width * meta.height * meta.bytesPerPixel();
        std::unique_ptr<uint8_t[]> tempPixels(new uint8_t[pixelCount]);

        stream.setPosition(0);
        stream.setSize(dataLength); // "Truncate" the stream to only include IDAT chunk data.

        if (!ProcessZLIB(stream, meta, tempPixels))
        {
            cerr << "PNG error: failed to process ZLIB" << endl;
            return Result();
        }

        benchEnd = clock();
        cout << (static_cast<int64_t>(benchEnd) - benchStart) / 1000.0 << " seconds (PROCESS DATA)" << endl;

        Result result;
        result.info = meta;
        result.pixels = tempPixels.release();
        return result;
    }

    bool PNG::ValidateParameters(const Metadata& meta)
    {
        // Checks if color type is valid and if the bit-depth is supported for the color type.
        uint8_t bits = meta.bitDepth;

        switch (meta.colorType)
        {
            case ColorType::Grayscale:
                return bits == 1 || bits == 2 || bits == 4 || bits == 8 || bits == 16;
            case ColorType::Indexed:
                return bits == 1 || bits == 2 || bits == 4 || bits == 8;
            case ColorType::Truecolor:
            case ColorType::GrayscaleWithAlpha:
            case ColorType::TruecolorWithAlpha:
                return bits == 8 || bits == 16;
            default:
                return false;
        }
    }

    bool PNG::ProcessZLIB(MemoryStream& stream, const Metadata& meta, const std::unique_ptr<uint8_t[]>& outPixels)
    {
        // ZLIB data specification (RFC-1950)
        uint16_t cmfFlg = stream.readUInt16();

        if (cmfFlg % 31 != 0)
        {
            cerr << "PNG file is corrupt: CMF_FLG is not divisible by 31" << endl;
            return false;
        }

        // CMF check: Look at first bits 0-3 to verify that the compression method is 8 (DEFLATE). That's the only supported method.
        // We don't care about other bits:
        //   bits 4-7: CINFO (determines LZ77 window size = 2^(8+CINFO))
        uint8_t cmf = static_cast<uint8_t>(cmfFlg >> 8);

        if ((cmf & 0x0f) != 8)
        {
            // NOTE: CMF = 15 is reserved, but not used at the time of implementing this.
            cerr << "PNG error: compression method is not DEFLATE (8)" << endl;
            return false;
        }

        // FLG check: Look at bit 5 (FDICT; preset dictionary) and ensure it's not set (specific to PNG implementation).
        // We don't care about other bits:
        //   bits 0-4 = FCHECK (already verified by checking if CMF_FLG is divisible by 31)
        //   bits 6-7 = FLEVEL (compression level used by compressor).
        uint8_t flg = static_cast<uint8_t>(cmfFlg);

        if ((flg & 0x20) != 0)
        {
            cerr << "PNG error: FDICT is set, which is unsupported" << endl;
            return false;
        }

        if (!Inflate(stream, meta, outPixels))
        {
            cerr << "Failed to inflate DEFLATE data" << endl;
            return false;
        }

        stream.advance(4);  // Skip ADLER32 (we already computed the data chunks' CRCs)
        return true;
    }

    bool PNG::Inflate(MemoryStream& stream, const Metadata& meta, const std::unique_ptr<uint8_t[]>& outPixels)
    {
        const size_t MINIMUM_DEFLATE_STREAM_SIZE = 32768;   // Should be no lower than maximum distance code value.

        // Static Huffman codes
        std::unique_ptr<CanonicalHuffman> staticLitLenCodes(new CanonicalHuffman(DEFLATE_STATIC_HUFFMAN_LIT_LEN_CODE_LENS.data(), DEFLATE_STATIC_HUFFMAN_LIT_LEN_CODE_LENS.size()));
        std::unique_ptr<CanonicalHuffman> staticDistCodes(new CanonicalHuffman(DEFLATE_STATIC_HUFFMAN_DIST_CODE_LENS.data(), DEFLATE_STATIC_HUFFMAN_DIST_CODE_LENS.size()));

        // Dynamic Huffman codes
        std::unique_ptr<CanonicalHuffman> dynamicLitLenCodes;   // Literal/length codes are required.
        std::unique_ptr<CanonicalHuffman> dynamicDistCodes;     // This can be null if dynamicLitLenCodes are all literals and no length codes.

        // Inflate (decompress) for DEFLATE data specification (RFC-1951).
        uint32_t scanlineWidth = (meta.bytesPerPixel() * meta.width) + 1;   // including filter type byte
        MemoryStream deflateStream(MINIMUM_DEFLATE_STREAM_SIZE);
        uint8_t* prevScanlinePtr = outPixels.get();
        uint8_t* currScanlinePtr = prevScanlinePtr;
        size_t streamScanlineOffset = 0;    // Position in deflateStream of which we will process the next scanline.

        bool isFinalBlock = false;
        while (!isFinalBlock)
        {
            // 3-bit header (BFINAL + BTYPE)
            isFinalBlock = stream.readBool();
            BlockType blockType = static_cast<BlockType>(stream.readUInt8(2));

            switch (blockType)
            {
                case BlockType::Uncompressed:
                {
                    stream.ceilToByteBoundary();        // Discard remaining bits in the current byte
                    uint16_t len = stream.readUInt16(MemoryStream::Endian::Little);
                    stream.advance(2);                  // We don't care about nlen (one's complement of len)
                    deflateStream.write(stream, len);   // Write 'len' literal bytes
                    stream.advance(len);
                    break;
                }

                case BlockType::StaticHuffman:
                {
                    if (!InflateHuffman(stream, deflateStream, staticLitLenCodes, staticDistCodes))
                    {
                        cerr << "Failed to parse static Huffman codes" << endl;
                        return false;
                    }
                    break;
                }

                case BlockType::DynamicHuffman:
                {
                    InflateParseHuffmanCodes(stream, dynamicLitLenCodes, dynamicDistCodes);

                    if (!dynamicLitLenCodes.get() || !InflateHuffman(stream, deflateStream, dynamicLitLenCodes, dynamicDistCodes))
                    {
                        cerr << "Failed to parse dynamic Huffman codes" << endl;
                        return false;
                    }
                    break;
                }

                default:
                {
                    cerr << "PNG error: invalid DEFLATE block type " << +static_cast<uint8_t>(blockType) << endl;
                    return false;
                }
            }

            size_t deflateStreamSize = deflateStream.size();
            size_t completeBytesCount = deflateStreamSize - (deflateStream.bitOffset() != 0);
            uint32_t scanlinesAvailable = (completeBytesCount - streamScanlineOffset) / scanlineWidth;

            if (scanlinesAvailable != 0)
            {
                size_t origStreamBytePos = deflateStream.byteOffset();
                uint8_t origStreamBitPos = deflateStream.bitOffset();

                // Process scanlines from wherever we left off last iteration.
                deflateStream.setPosition(streamScanlineOffset);

                if (!Unfilter(deflateStream, meta, prevScanlinePtr, currScanlinePtr, scanlinesAvailable))
                {
                    cerr << "PNG error: failed to unfilter DEFLATE data" << endl;
                    return false;
                }

                streamScanlineOffset += scanlinesAvailable * scanlineWidth;

                if (deflateStreamSize > MINIMUM_DEFLATE_STREAM_SIZE)
                {
                    // Discard any bytes beyond the maximum distance code value, since those aren't accessible or needed anymore.
                    // This will save a lot of memory, especially for large files.
                    size_t shiftOffset = deflateStreamSize - MINIMUM_DEFLATE_STREAM_SIZE;
                    uint8_t* streamStart = deflateStream.begin();
                    memcpy(streamStart, streamStart + shiftOffset, MINIMUM_DEFLATE_STREAM_SIZE);
                    deflateStream.setSize(MINIMUM_DEFLATE_STREAM_SIZE);

                    // We will need to adjust stream positions to be relative to the new stream starting position.
                    streamScanlineOffset -= shiftOffset;
                    origStreamBytePos -= shiftOffset;
                }

                // Restore stream pointer to where it was previously for the next iteration.
                deflateStream.setPosition(origStreamBytePos, origStreamBitPos);
            }
        }

        // Verify that we consumed all bytes within DEFLATE stream.
        if (deflateStream.byteOffset() != deflateStream.size() || deflateStream.bitOffset() != 0)
        {
            cerr << "PNG error: invalid DEFLATE stream position after INFLATE" << endl;
            return false;
        }

        return true;
    }

    void PNG::InflateParseHuffmanCodes(MemoryStream& stream, std::unique_ptr<CanonicalHuffman>& litLenCodes, std::unique_ptr<CanonicalHuffman>& distCodes)
    {
        // Decode dynamic Huffman block format defined in RFC-1951, Section 3.2.7.
        uint16_t litLenCodesCount = stream.readUInt8(5) + 257;  // HLIT + 257 (257-286)
        uint16_t distCodesCount = stream.readUInt8(5) + 1;      // HDIST + 1 (1-32)
        uint16_t codeLenCodesCount = stream.readUInt8(4) + 4;   // HCLEN + 4 (4-19)

        // Prepare and generate a Huffman table used to decode the literal/length/distance symbols.
        std::array<uint16_t, PNG_INIT_CODE_LENS_ARRAY_SIZE> codeLensCodeLens = {};

        for (uint16_t i = 0; i < codeLenCodesCount; i++)
        {
            codeLensCodeLens[PNG_INIT_CODE_LENS_ARRAY_FILL_ORDER[i]] = stream.readUInt8(3);
        }

        CanonicalHuffman codeLenHuffman = CanonicalHuffman(codeLensCodeLens.data(), PNG_INIT_CODE_LENS_ARRAY_SIZE);

        // Compute literals, lengths and distance codes (the actual code lengths) using the code lengths Huffman table.
        // Both literal/length and distance codes are stored in the same sequence.
        uint16_t totalCodeLenCount = litLenCodesCount + distCodesCount;
        std::unique_ptr<uint16_t[]> codeLens(new uint16_t[totalCodeLenCount]);
        uint16_t* codeLensPtr = codeLens.get();
        memset(codeLensPtr, 0, totalCodeLenCount * sizeof(uint16_t));
        uint16_t codeLensIndex = 0;

        while (codeLensIndex < totalCodeLenCount)
        {
            // Process each symbol. Alphabet is defined in RFC-1951, Section 3.2.7
            uint16_t symbol = codeLenHuffman.readNextSymbol(stream);
            uint16_t prevSymbol;
            uint8_t writeCount = 1;

            switch (symbol)
            {
                // 0 - 15 translates to the literal code lengths 0 - 15.
                case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15:
                    codeLens[codeLensIndex] = symbol;
                    break;
                case 16:
                    // Copy previous code length 3-6 times.
                    prevSymbol = codeLens[codeLensIndex - 1];
                    writeCount = 3 + stream.readUInt8(2);
                    std::fill_n(codeLensPtr + codeLensIndex, writeCount, prevSymbol);
                    break;
                case 17:
                    // Repeat code length '0' 3-10 times (aka no-op).
                    writeCount = 3 + stream.readUInt8(3);
                    break;
                case 18:
                    // Repeat code length '0' 11-138 times (aka skip).
                    writeCount = 11 + stream.readUInt8(7);
                    break;
                default:
                    cerr << "Invalid code length Huffman symbol " << symbol << endl;
                    return;
            }

            codeLensIndex += writeCount;
        }

        // Finally build the codes for literal/lengths.
        litLenCodes.reset(new CanonicalHuffman(codeLensPtr, litLenCodesCount));

        // Also build distance.
        uint16_t* distCodeLensPtr = codeLensPtr + litLenCodesCount;

        if (distCodesCount == 1 && distCodeLensPtr[0] == 0)
        {
            // One length of 0 bits indicates that there will be all literal data.
            distCodes.reset();
        }
        else
        {
            distCodes.reset(new CanonicalHuffman(distCodeLensPtr, distCodesCount));
        }
    }

    bool PNG::InflateHuffman(MemoryStream& stream, MemoryStream& outStream, const std::unique_ptr<CanonicalHuffman>& litLenCodes, const std::unique_ptr<CanonicalHuffman>& distCodes)
    {
        CanonicalHuffman* litLenCodesPtr = litLenCodes.get();
        CanonicalHuffman* distCodesPtr = distCodes.get();

        // Finally, actually decode ("inflate") the compressed data using these tables.
        while (stream.hasData())
        {
            uint16_t litLenSymbol = litLenCodesPtr->readNextSymbol(stream);

            if (litLenSymbol < 256)
            {
                outStream.writeUInt8(static_cast<uint8_t>(litLenSymbol));
            }
            else if (litLenSymbol == 256)
            {
                // 256 denotes end-of-block symbol
                return true;
            }
            else
            {
                // Special symbols will tell us how far back (distance code) and how much (length code) to copy.
                // These symbols are defined in 2 tables in RFC-1951, bottom of Section 3.2.5. One is for literals/lengths, other is for distances.
                uint16_t len;
                uint16_t dist;

                if (litLenSymbol <= 285)
                {
                    uint16_t index = litLenSymbol - 257;
                    len = DEFLATE_LIT_LEN_CODES_RANGE_START[index];
                    uint8_t extraBitsToRead = DEFLATE_LIT_LEN_CODES_EXTRA_BITS[index];
                    
                    if (extraBitsToRead != 0)
                        len += stream.readUInt8(extraBitsToRead);
                }
                else
                {
                    cerr << "PNG error: Failed to parse Huffman length symbol " << litLenSymbol << endl;
                    return false;
                }

                if (!distCodesPtr)
                {
                    // We must have distCodes provided at this point. Since not providing one will mean we can only accept literal bytes.
                    // We can't even step backwards in the input stream to copy anything...
                    cerr << "PNG file is corrupt: distance codes is null, but we are trying to parse code lengths" << endl;
                    return false;
                }

                uint16_t distSymbol = distCodesPtr->readNextSymbol(stream);

                if (distSymbol <= 29)
                {
                    dist = DEFLATE_DIST_CODES_RANGE_START[distSymbol];
                    uint8_t extraBitsToRead = DEFLATE_DIST_CODES_EXTRA_BITS[distSymbol];

                    if (extraBitsToRead != 0)
                        dist += stream.readUInt16(extraBitsToRead, MemoryStream::Endian::Little);
                }
                else
                {
                    cerr << "PNG error: Failed to parse Huffman distance symbol " << distSymbol << endl;
                    return false;
                }

                // Even though 'outStream.write()' allocates space for us already, we want to prevent the 'copyPtr' from changing
                // while we call 'outStream.write()'.
                outStream.allocateSpace(len);

                // Move back 'dist' bytes in 'outStream' and copy 'len' bytes to the end of buffer.
                uint8_t* copyPtr = outStream - dist;

                if (copyPtr < outStream.begin())
                {
                    cerr << "PNG error: attempted to copy bytes beyond the buffer start" << endl;
                    return false;
                }

                while (len != 0)
                {
                    if (len > dist)
                    {
                        // If length will exceed current outStream position, we repeat the bytes.
                        outStream.write(copyPtr, dist);
                        len -= dist;
                    }
                    else
                    {
                        // Can copy the block normally.
                        outStream.write(copyPtr, len);
                        len = 0;
                    }
                }
            }
        }

        cerr << "Failed to decode DEFLATE block using Huffman tables" << endl;
        return false;
    }

    bool PNG::Unfilter(MemoryStream& stream, const Metadata& meta, uint8_t*& prevScanline, uint8_t*& currScanline, uint32_t scanlineCount)
    {
        uint8_t bitDepth = meta.bitDepth;
        uint8_t bytesPerPixel = meta.bytesPerPixel();

        // Each scanline will start with a filter type byte, and then (bytesPerPixel * image width) more for the rest.
        uint32_t scanlineDataWidth = bytesPerPixel * meta.width;

        for (uint32_t line = 0; line < scanlineCount; line++)
        {
            FilterType filterType = static_cast<FilterType>(stream.readUInt8());
            stream.read(currScanline, scanlineDataWidth);

            switch (filterType)
            {
                case FilterType::None:
                    break;

                // Add pixel left
                case FilterType::Sub:
                {
                    for (uint32_t i = bytesPerPixel; i < scanlineDataWidth; i++)
                        currScanline[i] += currScanline[i - bytesPerPixel];

                    break;
                }

                // Add pixel above
                case FilterType::Up:
                {
                    if (prevScanline != currScanline)
                    {
                        for (uint32_t i = 0; i < scanlineDataWidth; i++)
                            currScanline[i] += prevScanline[i];
                    }

                    break;
                }

                // Add pixel average of above and left
                case FilterType::Average:
                {
                    if (prevScanline == currScanline)
                    {
                        for (uint32_t i = bytesPerPixel; i < scanlineDataWidth; i++)
                            currScanline[i] += currScanline[i - bytesPerPixel] / 2;
                    }
                    else
                    {
                        uint32_t i = 0;
                        for (; i < bytesPerPixel; i++)
                            currScanline[i] += prevScanline[i] / 2;
                        for (; i < scanlineDataWidth; i++)
                            currScanline[i] += (prevScanline[i] + currScanline[i - bytesPerPixel]) / 2;
                    }

                    break;
                }

                // Value closest to 'p = left + up - leftup'
                case FilterType::Paeth:
                {
                    if (prevScanline == currScanline)
                    {
                        // Previous row not available, so pixel to the left will always be the closest to 'p'.
                        for (uint32_t i = bytesPerPixel; i < scanlineDataWidth; i++)
                            currScanline[i] += currScanline[i - bytesPerPixel];
                    }
                    else
                    {
                        uint32_t i = 0;
                        // Left pixel not available, so pixels above will always be the closest to 'p'
                        for (; i < bytesPerPixel; i++)
                            currScanline[i] += prevScanline[i];
                        for (; i < scanlineDataWidth; i++)
                            currScanline[i] += PaethPredictor(currScanline[i - bytesPerPixel], prevScanline[i], prevScanline[i - bytesPerPixel]);
                    }

                    break;
                }

                default:
                    cerr << "PNG error: invalid filter type: " << static_cast<uint32_t>(filterType) << endl;
                    return false;
            }

            prevScanline = currScanline;
            currScanline += scanlineDataWidth;
        }

        return true;
    }

    uint8_t PNG::PaethPredictor(uint8_t left, uint8_t up, uint8_t upLeft)
    {
        uint16_t leftDiff = std::abs(up - upLeft);
        uint16_t upDiff = std::abs(left - upLeft);
        uint16_t upLeftDiff = std::abs(left + up - upLeft - upLeft);

        if (leftDiff <= upDiff && leftDiff <= upLeftDiff)
        {
            return left;
        }
        else if (upDiff <= upLeftDiff)
        {
            return up;
        }

        return upLeft;
    }
};
