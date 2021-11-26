#ifndef IMAGE_LIB_H
#define IMAGE_LIB_H

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <memory>

#include "Algorithms.h"
#include "../Serialization/MemoryStream.h"

using std::cout;
using std::cerr;
using std::endl;

namespace ImageLib
{
    static const uint8_t PNG_FILE_HEADER_SIZE = 16;
    static const uint8_t PNG_FILE_HEADER_INCL_DATA_SIZE = PNG_FILE_HEADER_SIZE + 13 + 4; // 13 = chunk length, 4 = CRC
    static const uint8_t PNG_FILE_HEADER[PNG_FILE_HEADER_SIZE] {
        0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, // magic numbers
        0x00, 0x00, 0x00, 0x0d, // chunk length (13)
        0x49, 0x48, 0x44, 0x52  // IHDR as chars
    };

    static const uint8_t PNG_INIT_CODE_LENS_ARRAY_SIZE = 19;
    static const uint8_t PNG_INIT_CODE_LENS_ARRAY_FILL_ORDER[PNG_INIT_CODE_LENS_ARRAY_SIZE]
    {
        16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
    };

    // Ranges are defined in tables in DEFLATE spec: RFC-1951, Section 3.2.5
    static const std::array<uint16_t, 29> DEFLATE_LIT_LEN_CODES_RANGE_START
    {
        3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
        35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258
    };
    static const std::array<uint8_t, 29> DEFLATE_LIT_LEN_CODES_EXTRA_BITS
    {
        0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
        3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0
    };

    static const std::array<uint16_t, 30> DEFLATE_DIST_CODES_RANGE_START
    {
        1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513,
        769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577
    };
    static const std::array<uint8_t, 30> DEFLATE_DIST_CODES_EXTRA_BITS
    {
        0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
        7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13
    };

    // PNG::BlockType::StaticHuffman
    static const std::array<uint16_t, 288> DEFLATE_STATIC_HUFFMAN_LIT_LEN_CODE_LENS
    {
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8
    };
    static const std::array<uint16_t, 32> DEFLATE_STATIC_HUFFMAN_DIST_CODE_LENS
    {
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5
    };

    class PNG
    {
        static const uint8_t MAX_HUFFMAN_CODE_LENGTH = 15;

        // Critical chunks.
        static const uint32_t CHUNK_PLTE = 1347179589;
        static const uint32_t CHUNK_IDAT = 1229209940;
        static const uint32_t CHUNK_IEND = 1229278788;

        // Ancillary chunks (not necessary for decoding).
        static const uint32_t CHUNK_tRNS = 1951551059;

        // Ignored ancillary chunks.
        static const uint32_t CHUNK_bKGD = 1649100612;
        static const uint32_t CHUNK_cHRM = 1665684045;
        static const uint32_t CHUNK_gAMA = 1732332865;
        static const uint32_t CHUNK_hIST = 1749635924;
        static const uint32_t CHUNK_iCCP = 1766015824;
        static const uint32_t CHUNK_iEXt = 1766152308;
        static const uint32_t CHUNK_iTXt = 1767135348;
        static const uint32_t CHUNK_pHYs = 1883789683;
        static const uint32_t CHUNK_sBIT = 1933723988;
        static const uint32_t CHUNK_sPLT = 1934642260;
        static const uint32_t CHUNK_sRGB = 1934772034;
        static const uint32_t CHUNK_tEXt = 1950701684;
        static const uint32_t CHUNK_tIME = 1950960965;
        static const uint32_t CHUNK_zTXt = 2052348020;

        enum class BlockType
        {
            Uncompressed = 0,
            StaticHuffman = 1,
            DynamicHuffman = 2,
            Reserved = 3
        };

        enum class ColorType
        {
            Grayscale = 0,
            Truecolor = 2,
            Indexed = 3,
            GrayscaleWithAlpha = 4,
            TruecolorWithAlpha = 6
        };

        enum class FilterType
        {
            // For some byte X: byte A = left of X; byte B = above X (prev scanline); byte C = left and above X
            None = 0,       // Byte X
            Sub = 1,        // Byte A
            Up = 2,         // Byte B
            Average = 3,    // (Byte A + B) / 2
            Paeth = 4       // Byte A, B, C, depending which is closer to (A + B - C)
        };

        struct Metadata
        {
            uint32_t width;
            uint32_t height;
            uint8_t bitDepth;
            ColorType colorType;
            bool interlaced;

            Metadata();

            bool hasAlpha() const
            {
                return colorType == ColorType::GrayscaleWithAlpha || colorType == ColorType::TruecolorWithAlpha;
            }

            uint8_t bitsPerPixel() const
            {
                switch (colorType)
                {
                    case ColorType::Grayscale:
                        return bitDepth;
                    case ColorType::Truecolor:
                        return bitDepth * 3;
                    case ColorType::Indexed:
                        return bitDepth;    // is it just 8?
                    case ColorType::GrayscaleWithAlpha:
                        return bitDepth * 2;
                    case ColorType::TruecolorWithAlpha:
                        return bitDepth * 4;
                }

                return 0;
            }

            uint8_t bytesPerPixel() const
            {
                return (bitsPerPixel() + 7) / 8;
            }
        };

        // Stores a Huffman tree in a compact, contiguous, and quick manner. Using an actual tree structure is too slow.
        class CanonicalHuffman
        {
        public:
            CanonicalHuffman() = delete;
            CanonicalHuffman(const uint16_t* codeLens, size_t codeLensCount);
            ~CanonicalHuffman();

            uint16_t readNextSymbol(MemoryStream& stream) const;
        private:
            uint16_t* m_SymbolCodes;    // Huffman codes w/ padded bit
            uint16_t* m_SymbolValues;   // Arbitrary value associated with the Huffman code.
            uint16_t m_SymbolCount;
        };

    public:
        struct Result
        {
            Metadata info;
            uint8_t* pixels;

            Result();

            inline bool isValid() { return pixels; }
        };

        static Result Load(const std::string& filePath);
        static bool ValidateParameters(const Metadata& meta);
        static bool ProcessZLIB(MemoryStream& stream, const Metadata& meta, const std::unique_ptr<uint8_t[]>& outPixels);
        static bool Inflate(MemoryStream& stream, const Metadata& meta, const std::unique_ptr<uint8_t[]>& outPixels);
        static void InflateParseHuffmanCodes(MemoryStream& stream, std::unique_ptr<CanonicalHuffman>& litLenCodes, std::unique_ptr<CanonicalHuffman>& distCodes);
        static bool InflateHuffman(MemoryStream& stream, MemoryStream& outStream, const std::unique_ptr<CanonicalHuffman>& litLenCodes, const std::unique_ptr<CanonicalHuffman>& distCodes);
        static bool Unfilter(MemoryStream& stream, const Metadata& meta, uint8_t*& prevScanline, uint8_t*& currScanline, uint32_t scanlineCount);
        static uint8_t PaethPredictor(uint8_t left, uint8_t up, uint8_t upLeft);
    };
};

#endif  // IMAGE_LIB_H
