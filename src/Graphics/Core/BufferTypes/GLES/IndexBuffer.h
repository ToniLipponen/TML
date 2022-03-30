#pragma once
#include "../../../../Headers/GLHeader.h"
#include <TML/Graphics/Core/Buffers.h>
#include <cstring>

namespace tml
{
    IndexBuffer::IndexBuffer() noexcept
    : m_id(0), m_elements(0), m_capacity(0)
    {
        GL_CALL(glad_glGenBuffers(1, &m_id));
    }

    IndexBuffer::IndexBuffer(const ui32* data, ui32 elements) noexcept
    : m_id(0), m_elements(0), m_capacity(elements)
    {
        if(data)
        {
            m_elements = m_capacity;
        }
        GL_CALL(glad_glGenBuffers(1, &m_id));
        Bind();
        GL_CALL(glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements * 4, data, GL_DYNAMIC_DRAW));
        m_mappedPtr = GL_CALL(glad_glMapNamedBuffer(m_id, MAP_RANGE_FLAGS));
        Unbind();
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

        Bind();
        GL_CALL(glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements * 4, data, GL_DYNAMIC_DRAW));
        m_mappedPtr = GL_CALL(glad_glMapNamedBuffer(m_id, MAP_RANGE_FLAGS));
        Unbind();
    }

    void IndexBuffer::PushData(const ui32* data, ui32 elements) noexcept
    {
        Bind();
        if(m_mappedPtr)
        {
            auto dest = ((ui32*) m_mappedPtr) + m_elements;
            std::memcpy(dest, data, elements*4);
            m_elements += elements;
        }
        Unbind();
    }

    void IndexBuffer::SetData(const ui32 *data, ui32 elements) noexcept
    {
        Flush();
        PushData(data, elements);
    }

    void IndexBuffer::Flush() noexcept
    {
        m_elements = 0;
    }
}