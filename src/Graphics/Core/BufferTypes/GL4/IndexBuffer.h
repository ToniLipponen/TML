#pragma once
#include "../../../../Headers/GLHeader.h"
#include <TML/Graphics/Core/Buffers.h>
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
        if(data)
            m_elements = elements;

        GL_CALL(glad_glCreateBuffers(1, &m_id));
        GL_CALL(glad_glNamedBufferStorage(m_id, m_capacity * 4, data, BUFFER_STORAGE_FLAGS));
        m_mappedPtr = GL_CALL(glad_glMapNamedBuffer(m_id, MAP_RANGE_FLAGS));
    }

    IndexBuffer::~IndexBuffer() noexcept
    {
        GL_CALL(glad_glDeleteBuffers(1, &m_id));
    }

    void IndexBuffer::Bind() const noexcept
    {

    }

    void IndexBuffer::Unbind() const noexcept
    {

    }

    void IndexBuffer::BufferData(const ui32* data, ui32 elements) noexcept
    {
        m_capacity = elements;
        if(data)
            m_elements = elements;

        GL_CALL(glad_glNamedBufferStorage(m_id, m_capacity * 4, data, BUFFER_STORAGE_FLAGS));
        if(m_mappedPtr)
        {
            GL_CALL(glad_glUnmapNamedBuffer(m_id));
        }
        m_mappedPtr = GL_CALL(glad_glMapNamedBuffer(m_id, MAP_RANGE_FLAGS));
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