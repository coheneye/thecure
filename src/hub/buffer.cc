#include <assert.h>

#include "buffer.h"


Buffer::Buffer(size_t size)
{
    assert(size > 0);
    m_size = size;
    m_buf = (char*)malloc(size * sizeof(char));
    assert(m_buf);
}


Buffer::~Buffer()
{
    if(m_buf){
        free(m_buf);
        m_buf = nullptr;
        m_size = 0;            
    }
}


void Buffer::resize(size_t size)
{
    if(size > m_size){
        m_size = size;
        m_buf = (char*)realloc(m_buf, size);
        assert(m_buf);
    }
}


char* Buffer::buf()
{
    return m_buf;
}


size_t Buffer::size()
{
    return m_size;
}