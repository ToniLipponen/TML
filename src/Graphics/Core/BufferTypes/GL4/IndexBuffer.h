#pragma once
#include <cstring>

namespace tml
{
    IndexBuffer::IndexBuffer() noexcept
    : m_id(0), m_elements(0), m_capacity(0)
    {
        GL_CALL(glad_glCreateBuffers(1, &m_id));
    }

    IndexBuffer::IndexBuffer(const ui32* data, ui32 elements) noexcept
    : m_id(0), m_elements(0), m_capacity(elements)
    {
        GL_CALL(glad_glCreateBuffers(1, &m_id));
        BufferData(data, elements);
    }

    IndexBuffer::~IndexBuffer() noexcept
    {
        GL_CALL(glad_glDeleteBuffers(1, &m_id));
    }

    void IndexBuffer::Bind() const noexcept
    {
        GL_CALL(glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
    }

    void IndexBuffer::Unbind() const noexcept
    {
        GL_CALL(glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

    void IndexBuffer::BufferData(const ui32* data, ui32 elements) noexcept
    {
        m_capacity = elements;

        if(data)
            m_elements = elements;
        else
            m_elements = 0;

        if(m_mappedPtr)
        {
            GL_CALL(glad_glUnmapNamedBuffer(m_id));
        }

        GL_CALL(glad_glNamedBufferData(m_id, m_capacity * 4, data, GL_STREAM_DRAW));
        m_mappedPtr = GL_CALL(glad_glMapNamedBufferRange(m_id, 0, m_capacity, MAP_RANGE_FLAGS | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_UNSYNCHRONIZED_BIT));
    }

    void IndexBuffer::PushData(const ui32* data, ui32 elements) noexcept
    {
        if(m_mappedPtr)
        {
            const ui32 size = elements * sizeof(ui32);
            auto dest = ((ui8 *) m_mappedPtr) + m_elements * sizeof(ui32);
            std::memcpy(dest, data, size);
            m_elements += elements;
        }
    }
}