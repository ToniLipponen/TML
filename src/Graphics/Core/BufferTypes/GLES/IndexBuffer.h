#pragma once
#include <TML/Graphics/Core/Buffers.h>
#include <cstring>
#include "../BufferFlags.h"
#include "../../../Headers/GLHeader.h"

namespace tml
{
    IndexBuffer::IndexBuffer() noexcept
    : m_id(0), m_elements(0), m_capacity(0)
    {
        GL_CALL(glad_glGenBuffers(1, &m_id));
    }

    IndexBuffer::IndexBuffer(const uint32_t* data, uint32_t elements) noexcept
    : m_id(0), m_elements(0), m_capacity(elements)
    {
        GL_CALL(glad_glGenBuffers(1, &m_id));
        BufferData(data, elements);
    }

    IndexBuffer::~IndexBuffer() noexcept
    {
        GL_CALL(glad_glDeleteBuffers(1, &m_id));
    }

    void IndexBuffer::Bind() noexcept
    {
        GL_CALL(glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
    }

    void IndexBuffer::Unbind() const noexcept
    {
        GL_CALL(glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

    void IndexBuffer::BufferData(const uint32_t* data, uint32_t elements) noexcept
    {
        m_capacity = elements;

        if(data)
        {
            m_elements = elements;
        }
        else
        {
            m_elements = 0;
        }

        Bind();
        GL_CALL(glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements * sizeof(uint32_t), data, BUFFER_USAGE_FLAG));
    }

    void IndexBuffer::PushData(const uint32_t* data, uint32_t elements) noexcept
    {
        const uint32_t size = elements * sizeof(uint32_t);
        const uint32_t offset = m_elements * sizeof(uint32_t);

        Bind();
        GL_CALL(glad_glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data));
        m_elements += elements;
    }
}