#include "MemoryStream.h"

MemoryStream::MemoryStream() : MemoryStream(DEFAULT_CAPACITY) { }
MemoryStream::MemoryStream(size_t capacity) : m_BitOffset(0), m_Size(0)
{
    capacity = std::max(DEFAULT_CAPACITY, capacity);
    m_DataStart = new uint8_t[capacity];
    m_CreatedData = true;
    m_DataEnd = m_DataStart;
    m_DataPtr = m_DataStart;
    m_Capacity = capacity;
}

MemoryStream::MemoryStream(uint8_t* buffer, size_t bufferSize, bool deleteBufferOnDestroy) : m_CreatedData(deleteBufferOnDestroy), m_BitOffset(0)
{
    m_DataStart = buffer;
    m_DataEnd = m_DataStart + bufferSize;
    m_DataPtr = m_DataStart;
    m_Capacity = bufferSize;
    m_Size = bufferSize;
}

MemoryStream::~MemoryStream()
{
    if (m_CreatedData)
        delete[] m_DataStart;

    m_Size = 0;
    m_Capacity = 0;
}

void MemoryStream::dumpToOstream(std::ostream& stream) const
{
    if (m_Size == 0)
        return;

    // Output full bytes first
    const char* data = reinterpret_cast<const char*>(m_DataStart);
    size_t fullBytesCount = m_Size;

    if (m_BitOffset != 0)
        fullBytesCount--;

    if (fullBytesCount > 0)
        stream.write(data, fullBytesCount);

    // Mask out remaining bits of the last byte.
    if (m_BitOffset != 0)
    {
        char lastByte = static_cast<char>(m_DataStart[fullBytesCount] & MASK_BITS(m_BitOffset));
        stream.put(lastByte);
    }
}
