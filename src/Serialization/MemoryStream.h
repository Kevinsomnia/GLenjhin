#ifndef MEMORY_STREAM_H
#define MEMORY_STREAM_H

#include <algorithm>
#include <exception>
#include <iostream>

#define MASK_BITS(numBits) ((1 << numBits) - 1)

using std::cout;
using std::cerr;
using std::endl;

class MemoryStream
{
public:
    static const size_t DEFAULT_CAPACITY = 256;

    enum class Endian
    {
        Little,
        Big
    };

    MemoryStream();
    MemoryStream(size_t capacity);
    MemoryStream(uint8_t* buffer, size_t bufferSize);
    ~MemoryStream();

    inline size_t byteOffset() const;
    inline uint8_t bitOffset() const;
    inline void setPosition(size_t byteOffset, uint8_t bitOffset = 0);
    inline void setPtrPosition(uint8_t* ptr);
    inline void setSize(size_t bytes);
    inline void advance(int64_t bytes, int64_t bits = 0);
    // Reset to previous byte boundary (sets bit offset to 0). Does nothing if we're already on one.
    inline void floorToByteBoundary();
    // Point to next available byte boundary. Does nothing if we're already on one.
    inline void ceilToByteBoundary();

    inline uint8_t* begin() const;
    inline uint8_t* end() const;
    inline size_t size() const;
    inline size_t capacity() const;
    inline bool hasData() const;
    inline void allocateSpace(uint32_t numBytes, uint32_t numBits = 0);
    inline uint8_t* getBufferCopy() const;

    operator uint8_t*() const { return m_DataPtr; }

    // === WRITING ===
    inline void writeBool(bool value);

    inline void writeUInt8(uint8_t value);
    inline void writeUInt8(uint8_t value, uint8_t numBits);
    inline void writeInt8(int8_t value);

    inline void writeUInt16(uint16_t value, Endian byteOrder = Endian::Big);
    inline void writeUInt16(uint16_t value, uint8_t numBits, Endian byteOrder = Endian::Big);
    inline void writeInt16(int16_t value, Endian byteOrder = Endian::Big);

    inline void writeUInt24(uint32_t value, Endian byteOrder = Endian::Big);
    inline void writeUInt24(uint32_t value, uint8_t numBits, Endian byteOrder = Endian::Big);
    inline void writeInt24(int32_t value, Endian byteOrder = Endian::Big);

    inline void writeUInt32(uint32_t value, Endian byteOrder = Endian::Big);
    inline void writeUInt32(uint32_t value, uint8_t numBits, Endian byteOrder = Endian::Big);
    inline void writeInt32(int32_t value, Endian byteOrder = Endian::Big);

    inline void writeUInt64(uint64_t value, Endian byteOrder = Endian::Big);
    inline void writeUInt64(uint64_t value, uint8_t numBits, Endian byteOrder = Endian::Big);
    inline void writeInt64(int64_t value, Endian byteOrder = Endian::Big);

    inline void writeFloat(float value);
    inline void writeDouble(double value);

    inline void write(uint8_t* data, uint32_t size);

    // === READING ===
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

    inline uint64_t readUInt64(Endian byteOrder = Endian::Big);
    inline uint64_t readUInt64(uint8_t numBits, Endian byteOrder = Endian::Big);
    inline int64_t readInt64(Endian byteOrder = Endian::Big);

    inline float readFloat();
    inline double readDouble();

    inline void read(uint8_t* buffer, uint32_t size);
private:
    bool m_CreatedData;
    uint8_t* m_DataStart;
    uint8_t* m_DataEnd;
    uint8_t* m_DataPtr;
    uint8_t m_BitOffset;
    size_t m_Capacity;
    size_t m_Size;

    inline void validateEnoughSpace(size_t bitsNeeded); // for reading only
    inline void expandCapacityIfNeeded(size_t bitsNeeded);  // for writing only
    inline void expandLengthIfNeeded(size_t bitsToWrite);   // for writing only
};



// =========================
// GENERAL FUNCTIONS
// =========================

size_t MemoryStream::byteOffset() const
{
    return static_cast<size_t>(m_DataPtr - m_DataStart);
}

uint8_t MemoryStream::bitOffset() const
{
    return m_BitOffset;
}

void MemoryStream::setPosition(size_t byteOffset, uint8_t bitOffset)
{
    m_DataPtr = m_DataStart + byteOffset;
    m_BitOffset = bitOffset & 7;
}

void MemoryStream::setPtrPosition(uint8_t* ptr)
{
    if (ptr < m_DataStart || ptr > m_DataEnd)
        throw std::out_of_range("MemoryStream: Attempting to set pointer position out of range");

    m_DataPtr = ptr;
    m_BitOffset = 0;
}

void MemoryStream::setSize(size_t bytes)
{
    if (bytes > m_Size)
    {
        size_t extraBits = (8 - m_BitOffset) & 7;
        extraBits += (m_Size - bytes) * 8;
        expandLengthIfNeeded(extraBits);
    }
    else
    {
        // Reduction will not clear bytes or shrink capacity/buffer.
        m_Size = bytes;
        m_DataEnd = m_DataStart + m_Size;

        // Clamp pointer.
        if (m_DataPtr >= m_DataEnd)
        {
            m_DataPtr = m_DataEnd;
            m_BitOffset = 0;
        }
    }
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

size_t MemoryStream::capacity() const
{
    return m_Capacity;
}

bool MemoryStream::hasData() const
{
    return m_DataPtr < m_DataEnd;
}

void MemoryStream::allocateSpace(uint32_t numBytes, uint32_t numBits)
{
    expandCapacityIfNeeded(static_cast<size_t>(numBytes * 8) + static_cast<size_t>(numBits));
}

uint8_t* MemoryStream::getBufferCopy() const
{
    uint8_t* result = new uint8_t[m_Size];
    memcpy(result, m_DataStart, m_Size);

    // Clear remaining bits in last byte if necessary (we don't automatically do this for performance)
    if (m_Size > 0 && m_BitOffset != 0)
        result[m_Size - 1] &= MASK_BITS(m_BitOffset);

    return result;
}


// =========================
// WRITING
// =========================

void MemoryStream::writeBool(bool value)
{
    expandLengthIfNeeded(1);

    if (value)
    {
        *m_DataPtr |= 1 << m_BitOffset;
    }
    else
    {
        *m_DataPtr &= ~(1 << m_BitOffset);
    }

    if (m_BitOffset == 7)
    {
        m_BitOffset = 0;
        m_DataPtr++;
    }
    else
    {
        m_BitOffset++;
    }
}

void MemoryStream::writeUInt8(uint8_t value)
{
    expandLengthIfNeeded(8);

    if (m_BitOffset == 0)
    {
        *m_DataPtr++ = value;
    }
    else
    {
        uint8_t leftoverBits = 8 - m_BitOffset;
        uint8_t firstByteMask = MASK_BITS(m_BitOffset);
        *m_DataPtr++ = (*m_DataPtr & firstByteMask) | (value << m_BitOffset);
        *m_DataPtr = (*m_DataPtr & ~firstByteMask) | (value >> leftoverBits);
    }
}

void MemoryStream::writeUInt8(uint8_t value, uint8_t numBits)
{
    if (numBits >= 8)
    {
        writeUInt8(value);
        return;
    }

    expandLengthIfNeeded(numBits);
    uint8_t bitsLeft = 8 - m_BitOffset;
    uint8_t valueBitMask = MASK_BITS(numBits);
    uint8_t maskedValue = value & valueBitMask;

    if (numBits <= bitsLeft)
    {
        uint8_t byteMask = ~(valueBitMask << m_BitOffset);
        *m_DataPtr = (*m_DataPtr & byteMask) | (maskedValue << m_BitOffset);

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
        uint8_t bitsToWriteSecondByte = numBits - bitsLeft;
        uint8_t firstByteMask = MASK_BITS(m_BitOffset);
        uint8_t secondByteMask = ~MASK_BITS(bitsToWriteSecondByte);

        *m_DataPtr++ = (*m_DataPtr & firstByteMask) | (maskedValue << m_BitOffset);
        *m_DataPtr = (*m_DataPtr & secondByteMask) | (maskedValue >> bitsLeft);
        m_BitOffset = bitsToWriteSecondByte;
    }
}

void MemoryStream::writeInt8(int8_t value)
{
    writeUInt8(static_cast<uint8_t>(value));
}

void MemoryStream::writeUInt16(uint16_t value, Endian byteOrder)
{
    if (byteOrder == Endian::Big)
    {
        writeUInt8(static_cast<uint8_t>(value >> 8));
        writeUInt8(static_cast<uint8_t>(value));
    }
    else
    {
        writeUInt8(static_cast<uint8_t>(value));
        writeUInt8(static_cast<uint8_t>(value >> 8));
    }
}

void MemoryStream::writeUInt16(uint16_t value, uint8_t numBits, Endian byteOrder)
{
    if (numBits <= 8)
    {
        writeUInt8(static_cast<uint8_t>(value), numBits);
    }
    else
    {
        uint8_t extraBits = numBits - 8;

        if (byteOrder == Endian::Big)
        {
            writeUInt8(static_cast<uint8_t>(value >> 8), extraBits);
            writeUInt8(static_cast<uint8_t>(value));
        }
        else
        {
            writeUInt8(static_cast<uint8_t>(value));
            writeUInt8(static_cast<uint8_t>(value >> 8), extraBits);
        }
    }
}

void MemoryStream::writeInt16(int16_t value, Endian byteOrder)
{
    writeUInt16(static_cast<uint16_t>(value), byteOrder);
}

void MemoryStream::writeUInt24(uint32_t value, Endian byteOrder)
{
    if (byteOrder == Endian::Big)
    {
        writeUInt8(static_cast<uint8_t>(value >> 16));
        writeUInt8(static_cast<uint8_t>(value >> 8));
        writeUInt8(static_cast<uint8_t>(value));
    }
    else
    {
        writeUInt8(static_cast<uint8_t>(value));
        writeUInt8(static_cast<uint8_t>(value >> 8));
        writeUInt8(static_cast<uint8_t>(value >> 16));
    }
}

void MemoryStream::writeUInt24(uint32_t value, uint8_t numBits, Endian byteOrder)
{
    if (numBits <= 16)
    {
        writeUInt16(static_cast<uint16_t>(value), numBits, byteOrder);
    }
    else
    {
        uint8_t extraBits = numBits - 16;

        if (byteOrder == Endian::Big)
        {
            writeUInt8(static_cast<uint8_t>(value >> 16), extraBits);
            writeUInt8(static_cast<uint8_t>(value >> 8));
            writeUInt8(static_cast<uint8_t>(value));
        }
        else
        {
            writeUInt8(static_cast<uint8_t>(value));
            writeUInt8(static_cast<uint8_t>(value >> 8));
            writeUInt8(static_cast<uint8_t>(value >> 16), extraBits);
        }
    }
}

void MemoryStream::writeInt24(int32_t value, Endian byteOrder)
{
    writeUInt24(static_cast<uint32_t>(value), byteOrder);
}

void MemoryStream::writeUInt32(uint32_t value, Endian byteOrder)
{
    if (byteOrder == Endian::Big)
    {
        writeUInt8(static_cast<uint8_t>(value >> 24));
        writeUInt8(static_cast<uint8_t>(value >> 16));
        writeUInt8(static_cast<uint8_t>(value >> 8));
        writeUInt8(static_cast<uint8_t>(value));
    }
    else
    {
        writeUInt8(static_cast<uint8_t>(value));
        writeUInt8(static_cast<uint8_t>(value >> 8));
        writeUInt8(static_cast<uint8_t>(value >> 16));
        writeUInt8(static_cast<uint8_t>(value >> 24));
    }
}

void MemoryStream::writeUInt32(uint32_t value, uint8_t numBits, Endian byteOrder)
{
    if (numBits <= 24)
    {
        writeUInt24(value, numBits, byteOrder);
    }
    else
    {
        uint8_t extraBits = numBits - 24;

        if (byteOrder == Endian::Big)
        {
            writeUInt8(static_cast<uint8_t>(value >> 24), extraBits);
            writeUInt8(static_cast<uint8_t>(value >> 16));
            writeUInt8(static_cast<uint8_t>(value >> 8));
            writeUInt8(static_cast<uint8_t>(value));
        }
        else
        {
            writeUInt8(static_cast<uint8_t>(value));
            writeUInt8(static_cast<uint8_t>(value >> 8));
            writeUInt8(static_cast<uint8_t>(value >> 16));
            writeUInt8(static_cast<uint8_t>(value >> 24), extraBits);
        }
    }
}

void MemoryStream::writeInt32(int32_t value, Endian byteOrder)
{
    writeUInt32(static_cast<uint32_t>(value), byteOrder);
}

void MemoryStream::writeUInt64(uint64_t value, Endian byteOrder)
{
    if (byteOrder == Endian::Big)
    {
        writeUInt32(static_cast<uint32_t>(value >> 32), Endian::Big);
        writeUInt32(static_cast<uint32_t>(value), Endian::Big);
    }
    else
    {
        writeUInt32(static_cast<uint32_t>(value), Endian::Little);
        writeUInt32(static_cast<uint32_t>(value >> 32), Endian::Little);
    }
}

void MemoryStream::writeUInt64(uint64_t value, uint8_t numBits, Endian byteOrder)
{
    if (numBits <= 32)
    {
        writeUInt32(static_cast<uint32_t>(value), numBits, byteOrder);
    }
    else
    {
        uint8_t extraBits = numBits - 32;

        if (byteOrder == Endian::Big)
        {
            writeUInt32(static_cast<uint32_t>(value >> 32), extraBits, MemoryStream::Endian::Big);
            writeUInt32(static_cast<uint32_t>(value), MemoryStream::Endian::Big);
        }
        else
        {
            writeUInt32(static_cast<uint32_t>(value), MemoryStream::Endian::Little);
            writeUInt32(static_cast<uint32_t>(value >> 32), extraBits, MemoryStream::Endian::Little);
        }
    }
}

void MemoryStream::writeInt64(int64_t value, Endian byteOrder)
{
    writeUInt64(static_cast<uint64_t>(value), byteOrder);
}

void MemoryStream::writeFloat(float value)
{
    writeUInt32(*reinterpret_cast<uint32_t*>(&value));
}

void MemoryStream::writeDouble(double value)
{
    writeUInt64(*reinterpret_cast<uint64_t*>(&value));
}

void MemoryStream::write(uint8_t* data, uint32_t size)
{
    if (size == 0)
        return;

    if (m_BitOffset == 0)
    {
        expandLengthIfNeeded(size * 8);
        memcpy(m_DataPtr, data, size);
        m_DataPtr += size;
    }
    else
    {
        // Very slow... recommended to align to byte.
        for (uint32_t i = 0; i < size; i++)
            writeUInt8(data[i]);
    }
}


// =========================
// READING
// =========================

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
    uint8_t a, b;

    if (byteOrder == Endian::Big)
    {
        a = readUInt8();
        b = readUInt8();
    }
    else
    {
        b = readUInt8();
        a = readUInt8();
    }

    return (static_cast<uint16_t>(a) << 8) | b;
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

        uint8_t extraBits = numBits - 8;
        uint8_t a, b;

        if (byteOrder == Endian::Big)
        {
            a = readUInt8(extraBits);
            b = readUInt8();
        }
        else
        {
            b = readUInt8();
            a = readUInt8(extraBits);
        }

        return (static_cast<uint16_t>(a) << 8) | b;
    }
}

int16_t MemoryStream::readInt16(Endian byteOrder)
{
    return static_cast<int16_t>(readUInt16(byteOrder));
}

uint32_t MemoryStream::readUInt24(Endian byteOrder)
{
    validateEnoughSpace(24);
    uint8_t a, b, c;

    if (byteOrder == Endian::Big)
    {
        a = readUInt8();
        b = readUInt8();
        c = readUInt8();

    }
    else
    {
        c = readUInt8();
        b = readUInt8();
        a = readUInt8();
    }

    return (static_cast<uint32_t>(a) << 16) | (static_cast<uint32_t>(b) << 8) | c;
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

        uint8_t extraBits = numBits - 16;
        uint8_t a, b, c;

        if (byteOrder == Endian::Big)
        {
            a = readUInt8(extraBits);
            b = readUInt8();
            c = readUInt8();
        }
        else
        {
            c = readUInt8();
            b = readUInt8();
            a = readUInt8(extraBits);
        }

        return (static_cast<uint32_t>(a) << 16) | (static_cast<uint32_t>(b) << 8) | c;
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

        uint8_t extraBits = numBits - 24;
        uint8_t a, b, c, d;

        if (byteOrder == Endian::Big)
        {
            a = readUInt8(extraBits);
            b = readUInt8();
            c = readUInt8();
            d = readUInt8();
        }
        else
        {
            d = readUInt8();
            c = readUInt8();
            b = readUInt8();
            a = readUInt8(extraBits);
        }

        return (static_cast<uint32_t>(a) << 24) |
                (static_cast<uint32_t>(b) << 16) |
                (static_cast<uint32_t>(c) << 8) |
                d;
    }
}

int32_t MemoryStream::readInt32(Endian byteOrder)
{
    return static_cast<int32_t>(readUInt32(byteOrder));
}

uint64_t MemoryStream::readUInt64(Endian byteOrder)
{
    validateEnoughSpace(64);
    uint32_t a, b;

    if (byteOrder == Endian::Big)
    {
        a = readUInt32();
        b = readUInt32();
    }
    else
    {
        b = readUInt32(Endian::Little);
        a = readUInt32(Endian::Little);
    }

    return (static_cast<uint64_t>(a) << 32) | b;
}

uint64_t MemoryStream::readUInt64(uint8_t numBits, Endian byteOrder)
{
    if (numBits <= 32)
    {
        return static_cast<uint64_t>(readUInt32(numBits, byteOrder));
    }
    else
    {
        validateEnoughSpace(numBits);

        uint8_t extraBits = numBits - 32;
        uint32_t a, b;

        if (byteOrder == Endian::Big)
        {
            a = readUInt32(extraBits);
            b = readUInt32();
        }
        else
        {
            b = readUInt32(Endian::Little);
            a = readUInt32(extraBits, Endian::Little);
        }

        return (static_cast<uint64_t>(a) << 32) | b;
    }
}

int64_t MemoryStream::readInt64(Endian byteOrder)
{
    return static_cast<int64_t>(readUInt64(byteOrder));
}

float MemoryStream::readFloat()
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
        uint32_t bytes = readUInt32();
        return *reinterpret_cast<float*>(&bytes);
    }
}

double MemoryStream::readDouble()
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
        uint64_t bytes = readUInt64();
        return *reinterpret_cast<double*>(&bytes);
    }
}

void MemoryStream::read(uint8_t* buffer, uint32_t size)
{
    if (m_BitOffset == 0)
    {
        validateEnoughSpace(size * 8);
        memcpy(buffer, m_DataPtr, size);
        m_DataPtr += size;
    }
    else
    {
        // Very slow... recommended to align to byte.
        for (uint32_t i = 0; i < size; i++)
            buffer[i] = readUInt8();
    }
}


// =========================
// PRIVATE INTERFACE
// =========================

void MemoryStream::validateEnoughSpace(size_t bitsNeeded)
{
    size_t bitsLeft = ((m_DataEnd - m_DataPtr) * 8) - m_BitOffset;

    if (bitsNeeded > bitsLeft)
        throw std::out_of_range("MemoryStream: Not enough bits left");
}

void MemoryStream::expandCapacityIfNeeded(size_t bitsNeeded)
{
    size_t bitsLeftInBuffer = (((m_DataStart + m_Capacity) - m_DataPtr) * 8) - m_BitOffset;

    if (bitsNeeded > bitsLeftInBuffer)
    {
        size_t extraBits = bitsNeeded - bitsLeftInBuffer;
        size_t bytesNeeded = (extraBits / 8) + (extraBits % 8 != 0);    // Integer ceil()
        size_t minCapacity = m_Capacity + bytesNeeded;
        size_t newCapacity = m_Capacity;

        while (newCapacity < minCapacity)
            newCapacity <<= 1;

        size_t origByteOffset = byteOffset();

        // Allocate a new buffer. Copy old buffer contents over. Delete old buffer.
        uint8_t* newBuffer = new uint8_t[newCapacity];
        memcpy(newBuffer, m_DataStart, m_Size);

        if (m_CreatedData)
            delete[] m_DataStart;

        // Update remaining properties.
        m_DataStart = newBuffer;
        m_DataPtr = m_DataStart + origByteOffset;
        m_DataEnd = m_DataStart + m_Size;
        m_Capacity = newCapacity;
        m_CreatedData = true;
    }
}

void MemoryStream::expandLengthIfNeeded(size_t bitsToWrite)
{
    size_t newEndBitOffset = ((m_DataPtr - m_DataStart) * 8) + m_BitOffset + bitsToWrite;
    size_t newEndByteOffset = (newEndBitOffset / 8) + (newEndBitOffset % 8 != 0);   // Integer ceil()

    // If we're gonna write past m_DataEnd.
    if (m_DataStart + newEndByteOffset > m_DataEnd)
    {
        // Update capacity if needed.
        if (newEndByteOffset > m_Capacity)
            expandCapacityIfNeeded(bitsToWrite);

        // Update size.
        m_DataEnd = m_DataStart + newEndByteOffset;
        m_Size = newEndByteOffset;
    }
}

#endif  // MEMORY_STREAM_H
