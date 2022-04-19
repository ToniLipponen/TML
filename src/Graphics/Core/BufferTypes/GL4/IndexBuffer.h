#pragma once
#include <cstring>

namespace tml
{
    IndexBuffer::IndexBuffer() noexcept
    : m_id(0), m_elements(0), m_capacity(0)
    {
        GL_CALL(glad_glCreateBuffers(1, &m_id));
    }

    IndexBuffer::IndexBuffer(const uint32_t* data, uint32_t elements) noexcept
    : m_id(0), m_elements(0), m_capacity(elements)
    {
        GL_CALL(glad_glCreateBuffers(1, &m_id));
        BufferData(data, elements);
    }

    IndexBuffer::~IndexBuffer() noexcept
    {
        GL_CALL(glad_glDeleteBuffers(1, &m_id));
    }

    void IndexBuffer::Bind() noexcept
    {
        if(m_mappedPtr)
        {
//            GL_CALL(glad_glFlushMappedNamedBufferRange(m_id, 0, m_elements*4));
            GL_CALL(glad_glUnmapNamedBuffer(m_id));
            m_mappedPtr = nullptr;
        }
    }

    void IndexBuffer::Unbind() const noexcept
    {

    }

    void IndexBuffer::BufferData(const uint32_t* data, uint32_t elements) noexcept
    {
        if(m_mappedPtr)
        {
//            GL_CALL(glad_glFlushMappedNamedBufferRange(m_id, 0, m_elements*4));
            GL_CALL(glad_glUnmapNamedBuffer(m_id));
        }

        m_capacity = elements;
        const auto size = elements * 4;

        if(data)
            m_elements = elements;
        else
            m_elements = 0;

        GL_CALL(glad_glNamedBufferData(m_id, size, data, BUFFER_USAGE_FLAG));
        m_mappedPtr = GL_CALL(glad_glMapNamedBufferRange(m_id, 0, size, BUFFER_MAP_FLAGS));
    }

    void IndexBuffer::PushData(const uint32_t* data, uint32_t elements) noexcept
    {
        if(m_mappedPtr)
        {
            const uint32_t size = elements * sizeof(uint32_t);
            auto dest = ((uint8_t*) m_mappedPtr) + m_elements * sizeof(uint32_t);
            std::memcpy(dest, data, size);
            m_elements += elements;
        }
    }
}