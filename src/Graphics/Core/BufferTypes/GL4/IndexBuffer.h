#pragma once
#include <TML/Graphics/Core/IndexBuffer.h>
#include <cstring>
#include "../BufferFlags.h"
#include "../../../Headers/GLHeader.h"

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

    }

    void IndexBuffer::Unbind() const noexcept
    {

    }

    void IndexBuffer::BufferData(const uint32_t* data, uint32_t elements) noexcept
    {
        m_capacity = elements;
        const auto size = elements * 4;

        if(data)
        {
            m_elements = elements;
        }
        else
        {
            m_elements = 0;
        }

        GL_CALL(glad_glNamedBufferData(m_id, size, data, BUFFER_USAGE_FLAG));
    }

    void IndexBuffer::PushData(const uint32_t* data, uint32_t elements) noexcept
    {
        const uint32_t size = elements * sizeof(uint32_t);
        const uint32_t offset = m_elements * sizeof(uint32_t);

        GL_CALL(glad_glNamedBufferSubData(m_id, offset, size, data));
        m_elements += elements;
    }
}