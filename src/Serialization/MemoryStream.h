#ifndef MEMORY_STREAM_H
#define MEMORY_STREAM_H

#include <iostream>
#include <exception>

#define MASK_BITS(numBits) (1 << numBits) - 1

using std::cout;
using std::endl;

class MemoryStream
{
public:
    enum class Endian
    {
        Little,
        Big
    };

    MemoryStream() = delete;
    MemoryStream(uint8_t* buffer, size_t bufferSize);

    inline size_t getByteOffset() const;
    inline uint8_t getBitOffset() const;
    inline void setPosition(size_t byteOffset, uint8_t bitOffset = 0);
    inline void advance(int64_t bytes, int64_t bits = 0);
    // Reset to previous byte boundary (sets bit offset to 0). Does nothing if we're already on one.
    inline void floorToByteBoundary();
    // Point to next available byte boundary. Does nothing if we're already on one.
    inline void ceilToByteBoundary();

    inline uint8_t* begin() const;
    inline uint8_t* end() const;
    inline size_t size() const;
    inline bool hasData() const;

    operator uint8_t*() const { return m_DataPtr; }

    inline bool readBool();

    inline uint8_t readUInt8();
    inline uint8_t readUInt8(uint8_t numBits);
    inline int8_t readInt8();

    inline uint16_t readUInt16(Endian byteOrder = Endian::Big);
    inline uint16_t readUInt16(uint8_t numBits, Endian byteOrder = Endian::Big);
    inline int16_t readInt16(Endian byteOrder = Endian::Big);

    inline uint32_t readUInt24(Endian byteOrder = Endian::Big);
    inline uint32_t readUInt24(uint8_t numBits, Endian byteOrder = Endian::Big);
    inline int32_t readInt24(Endian byteOrder = Endian::Big);

    inline uint32_t readUInt32(Endian byteOrder = Endian::Big);
    inline uint32_t readUInt32(uint8_t numBits, Endian byteOrder = Endian::Big);
    inline int32_t readInt32(Endian byteOrder = Endian::Big);

    inline float readFloat(Endian byteOrder = Endian::Big);

    inline uint64_t readUInt64(Endian byteOrder = Endian::Big);
    inline uint64_t readUInt64(uint8_t numBits, Endian byteOrder = Endian::Big);
    inline int64_t readInt64(Endian byteOrder = Endian::Big);

    inline double readDouble(Endian byteOrder = Endian::Big);
private:
    uint8_t* m_DataStart;
    uint8_t* m_DataEnd;
    uint8_t* m_DataPtr;
    uint8_t m_BitOffset;
    size_t m_Size;

    inline void validateEnoughSpace(size_t bitsNeeded);
};

size_t MemoryStream::getByteOffset() const
{
    return static_cast<size_t>(m_DataPtr - m_DataStart);
}

uint8_t MemoryStream::getBitOffset() const
{
    return m_BitOffset;
}

void MemoryStream::setPosition(size_t byteOffset, uint8_t bitOffset)
{
    m_DataPtr = m_DataStart + byteOffset;
    m_BitOffset = bitOffset & 7;
}

void MemoryStream::advance(int64_t bytes, int64_t bits)
{
    m_DataPtr += bytes;

    if (bits != 0)
    {
        int64_t newBitOffset = m_BitOffset + bits;

        if (newBitOffset >= 8)
        {
            m_DataPtr += newBitOffset / 8;
            m_BitOffset = newBitOffset & 7; // modulo 8
        }
        else if (newBitOffset < 0)
        {
            int64_t bytesBack = static_cast<int64_t>(ceil(-newBitOffset / 8) + 0.0001);
            m_DataPtr -= bytesBack;
            m_BitOffset = static_cast<uint8_t>(8 + newBitOffset + (bytesBack * 8)); // Calculate new bit offset after adding byte offset.
        }
        else
        {
            m_BitOffset = static_cast<uint8_t>(newBitOffset);
        }
    }
}

void MemoryStream::floorToByteBoundary()
{
    m_BitOffset = 0;
}

void MemoryStream::ceilToByteBoundary()
{
    if (m_BitOffset != 0)
    {
        m_DataPtr++;
        m_BitOffset = 0;
    }
}

uint8_t* MemoryStream::begin() const
{
    return m_DataStart;
}

uint8_t* MemoryStream::end() const
{
    return m_DataEnd;
}

size_t MemoryStream::size() const
{
    return m_Size;
}

bool MemoryStream::hasData() const
{
    return m_DataPtr < m_DataEnd;
}

bool MemoryStream::readBool()
{
    validateEnoughSpace(1);
    bool result = ((*m_DataPtr >> m_BitOffset) & 1) == 1;

    if (m_BitOffset == 7)
    {
        m_BitOffset = 0;
        m_DataPtr++;
    }
    else
    {
        m_BitOffset++;
    }

    return result;
}

uint8_t MemoryStream::readUInt8()
{
    validateEnoughSpace(8);

    if (m_BitOffset == 0)
    {
        return *m_DataPtr++;
    }
    else
    {
        uint8_t currByte = *m_DataPtr++;
        uint8_t nextByte = *m_DataPtr;
        uint8_t bitsLeft = 8 - m_BitOffset;
        return ((nextByte & MASK_BITS(m_BitOffset)) << bitsLeft) | ((currByte >> m_BitOffset) & MASK_BITS(bitsLeft));
    }
}

uint8_t MemoryStream::readUInt8(uint8_t numBits)
{
    validateEnoughSpace(numBits);

    uint8_t result;
    uint8_t bitsLeft = 8 - m_BitOffset;
    
    if (numBits <= bitsLeft)
    {
        result = (*m_DataPtr >> m_BitOffset) & MASK_BITS(numBits);

        if (numBits == bitsLeft)
        {
            m_DataPtr++;
            m_BitOffset = 0;
        }
        else
        {
            m_BitOffset += numBits;
        }
    }
    else
    {
        // Spans 2 bytes.
        result = (*m_DataPtr++ >> m_BitOffset) & MASK_BITS(bitsLeft);
        uint8_t leftoverBits = numBits - bitsLeft;
        result |= (*m_DataPtr & MASK_BITS(leftoverBits)) << bitsLeft;
        m_BitOffset = leftoverBits;
    }

    return result;
}

int8_t MemoryStream::readInt8()
{
    return static_cast<int8_t>(readUInt8());
}

uint16_t MemoryStream::readUInt16(Endian byteOrder)
{
    validateEnoughSpace(16);

    uint16_t result;

    if (byteOrder == Endian::Big)
    {
        result = (static_cast<uint16_t>(readUInt8()) << 8) |
                readUInt8();
    }
    else
    {
        result = readUInt8() |
                (static_cast<uint16_t>(readUInt8()) << 8);
    }

    return result;
}

uint16_t MemoryStream::readUInt16(uint8_t numBits, Endian byteOrder)
{
    if (numBits <= 8)
    {
        return static_cast<uint16_t>(readUInt8(numBits));
    }
    else
    {
        validateEnoughSpace(numBits);

        uint16_t result;
        uint8_t extraBits = numBits - 8;

        if (byteOrder == Endian::Big)
        {
            result = (static_cast<uint16_t>(readUInt8(extraBits)) << 8) |
                    readUInt8();
        }
        else
        {
            result = readUInt8() |
                    (static_cast<uint16_t>(readUInt8(extraBits)) << 8);
        }

        return result;
    }
}

int16_t MemoryStream::readInt16(Endian byteOrder)
{
    return static_cast<int16_t>(readUInt16(byteOrder));
}

uint32_t MemoryStream::readUInt24(Endian byteOrder)
{
    validateEnoughSpace(24);

    uint32_t result;

    if (byteOrder == Endian::Big)
    {
        result = (static_cast<uint32_t>(readUInt8()) << 16) |
                (static_cast<uint32_t>(readUInt8()) << 8) |
                readUInt8();
    }
    else
    {
        result = readUInt8() |
                (static_cast<uint32_t>(readUInt8()) << 8) |
                (static_cast<uint32_t>(readUInt8()) << 16);
    }

    return result;
}

uint32_t MemoryStream::readUInt24(uint8_t numBits, Endian byteOrder)
{
    if (numBits <= 16)
    {
        return static_cast<uint32_t>(readUInt16(numBits, byteOrder));
    }
    else
    {
        validateEnoughSpace(numBits);

        uint32_t result;
        uint8_t extraBits = numBits - 16;

        if (byteOrder == Endian::Big)
        {
            result = (static_cast<uint32_t>(readUInt8(extraBits)) << 16) |
                    (static_cast<uint32_t>(readUInt8()) << 8) |
                    readUInt8();
        }
        else
        {
            result = readUInt8() |
                    (static_cast<uint32_t>(readUInt8()) << 8) |
                    (static_cast<uint32_t>(readUInt8(extraBits)) << 16);
        }

        return result;
    }
}

int32_t MemoryStream::readInt24(Endian byteOrder)
{
    return static_cast<int32_t>(readUInt32(byteOrder));
}

uint32_t MemoryStream::readUInt32(Endian byteOrder)
{
    validateEnoughSpace(32);

    uint32_t result;

    if (byteOrder == Endian::Big)
    {
        result = (static_cast<uint32_t>(readUInt8()) << 24) |
                (static_cast<uint32_t>(readUInt8()) << 16) |
                (static_cast<uint32_t>(readUInt8()) << 8) |
                readUInt8();
    }
    else
    {
        result = readUInt8() |
                (static_cast<uint32_t>(readUInt8()) << 8) |
                (static_cast<uint32_t>(readUInt8()) << 16) |
                (static_cast<uint32_t>(readUInt8()) << 24);
    }

    return result;
}

uint32_t MemoryStream::readUInt32(uint8_t numBits, Endian byteOrder)
{
    if (numBits <= 24)
    {
        return readUInt24(numBits, byteOrder);
    }
    else
    {
        validateEnoughSpace(numBits);

        uint32_t result;
        uint8_t extraBits = numBits - 24;

        if (byteOrder == Endian::Big)
        {
            result = (static_cast<uint32_t>(readUInt8(extraBits)) << 24) |
                    (static_cast<uint32_t>(readUInt8()) << 16) |
                    (static_cast<uint32_t>(readUInt8()) << 8) |
                    readUInt8();
        }
        else
        {
            result = readUInt8() |
                    (static_cast<uint32_t>(readUInt8()) << 8) |
                    (static_cast<uint32_t>(readUInt8()) << 16) |
                    (static_cast<uint32_t>(readUInt8(extraBits)) << 24);
        }

        return result;
    }
}

int32_t MemoryStream::readInt32(Endian byteOrder)
{
    return static_cast<int32_t>(readUInt32(byteOrder));
}

float MemoryStream::readFloat(Endian byteOrder)
{
    validateEnoughSpace(32);

    if (m_BitOffset == 0)
    {
        float result = *reinterpret_cast<float*>(m_DataPtr);
        m_DataPtr += 4;
        return result;
    }
    else
    {
        uint32_t bytes = readUInt32(byteOrder);
        return *reinterpret_cast<float*>(&bytes);
    }
}

uint64_t MemoryStream::readUInt64(Endian byteOrder)
{
    validateEnoughSpace(64);

    uint64_t result;

    if (byteOrder == Endian::Big)
    {
        result = (static_cast<uint64_t>(readUInt8()) << 56) |
                (static_cast<uint64_t>(readUInt8()) << 48) |
                (static_cast<uint64_t>(readUInt8()) << 40) |
                (static_cast<uint64_t>(readUInt8()) << 32) |
                (static_cast<uint64_t>(readUInt8()) << 24) |
                (static_cast<uint64_t>(readUInt8()) << 16) |
                (static_cast<uint64_t>(readUInt8()) << 8) |
                readUInt8();
    }
    else
    {
        result = readUInt8() |
                (static_cast<uint64_t>(readUInt8()) << 8) |
                (static_cast<uint64_t>(readUInt8()) << 16) |
                (static_cast<uint64_t>(readUInt8()) << 24) |
                (static_cast<uint64_t>(readUInt8()) << 32) |
                (static_cast<uint64_t>(readUInt8()) << 40) |
                (static_cast<uint64_t>(readUInt8()) << 48) |
                (static_cast<uint64_t>(readUInt8()) << 56);
    }

    return result;
}

uint64_t MemoryStream::readUInt64(uint8_t numBits, Endian byteOrder)
{
    if (numBits <= 32)
    {
        return static_cast<uint64_t>(readUInt32(numBits));
    }
    else
    {
        validateEnoughSpace(numBits);

        uint64_t result;
        uint8_t extraBits = numBits - 32;

        if (byteOrder == Endian::Big)
        {
            result = (static_cast<uint64_t>(readUInt32(extraBits, Endian::Big)) << 32) | static_cast<uint64_t>(readUInt32(Endian::Big));
        }
        else
        {
            result = static_cast<uint64_t>(readUInt32(Endian::Little)) | (static_cast<uint64_t>(readUInt32(extraBits, Endian::Little)) << 32);
        }

        return result;
    }
}

int64_t MemoryStream::readInt64(Endian byteOrder)
{
    return static_cast<int64_t>(readUInt64(byteOrder));
}

double MemoryStream::readDouble(Endian byteOrder)
{
    validateEnoughSpace(64);

    if (m_BitOffset == 0)
    {
        double result = *reinterpret_cast<double*>(m_DataPtr);
        m_DataPtr += 8;
        return result;
    }
    else
    {
        uint64_t bytes = readUInt64(byteOrder);
        return *reinterpret_cast<double*>(&bytes);
    }
}

void MemoryStream::validateEnoughSpace(size_t bitsNeeded)
{
    size_t bitsLeft = ((m_DataEnd - m_DataPtr) * 8) - m_BitOffset;

    if (bitsNeeded > bitsLeft)
        throw std::out_of_range("MemoryStream: Not enough bits left");
}

#endif  // MEMORY_STREAM_H
