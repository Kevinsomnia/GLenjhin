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

MemoryStream::MemoryStream(uint8_t* buffer, size_t bufferSize) : m_BitOffset(0)
{
    m_DataStart = buffer;
    m_CreatedData = false;
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
