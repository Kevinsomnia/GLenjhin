#include "MemoryStream.h"

MemoryStream::MemoryStream(uint8_t* buffer, size_t bufferSize) : m_BitOffset(0)
{
    m_DataStart = buffer;
    m_DataEnd = m_DataStart + bufferSize;
    m_DataPtr = m_DataStart;
    m_Size = bufferSize;
}
