#pragma once
#include "../../../../Headers/GLHeader.h"
#include <TML/Graphics/Core/Buffers.h>
#include <cstring>

namespace tml
{
    VertexBuffer::VertexBuffer() noexcept
            : m_id(0), m_dataSize(0), m_vertexCount(0), m_capacity(0)
    {
        GL_CALL(glad_glCreateBuffers(1, &m_id));
    }

    VertexBuffer::VertexBuffer(const void* data, ui32 vertexSize, ui32 vertexCount) noexcept
            : m_id(0), m_dataSize(0), m_vertexCount(0), m_capacity(vertexSize * vertexCount)
    {
        if(data)
        {
            m_vertexCount = vertexCount;
            m_dataSize = m_capacity;
        }
        GL_CALL(glad_glCreateBuffers(1, &m_id));
        GL_CALL(glad_glNamedBufferStorage(m_id, m_capacity, data, BUFFER_STORAGE_FLAGS));
        m_mappedPtr = GL_CALL(glad_glMapNamedBuffer(m_id, MAP_RANGE_FLAGS));
    }

    VertexBuffer::~VertexBuffer() noexcept
    {
        GL_CALL(glad_glDeleteBuffers(1, &m_id));
    }

    void VertexBuffer::Bind() const noexcept
    {

    }

    void VertexBuffer::Unbind() const noexcept
    {

    }

    void VertexBuffer::BufferData(void* data, ui32 vertexSize, ui32 vertexCount) noexcept
    {
        m_capacity = vertexSize * vertexCount;

        if(data)
        {
            m_vertexCount = vertexCount;
            m_dataSize = m_capacity;
        }
        else
        {
            m_vertexCount = 0;
            m_dataSize = 0;
        }

        GL_CALL(glad_glNamedBufferStorage(m_id, m_capacity, data, BUFFER_STORAGE_FLAGS));

        if(m_mappedPtr)
        {
            GL_CALL(glad_glUnmapNamedBuffer(m_id));
        }
        m_mappedPtr = GL_CALL(glad_glMapNamedBuffer(m_id, MAP_RANGE_FLAGS));
    }

    void VertexBuffer::SetData(void *data, ui32 s, ui32 n) noexcept
    {
        m_dataSize = s * n;
        GL_CALL(glad_glNamedBufferSubData(m_id, 0, m_dataSize, data));
    }

    void VertexBuffer::PushData(const void* data, ui32 vertexSize, ui32 vertexCount) noexcept
    {
        if(m_mappedPtr)
        {
            const ui32 size = vertexSize * vertexCount;
            auto dest = ((ui8 *) m_mappedPtr) + m_dataSize;
            std::memcpy(dest, data, size);
            m_dataSize += size;
            m_vertexCount += vertexCount;
        }
    }

    void VertexBuffer::Flush() noexcept
    {
        m_dataSize = 0;
        m_vertexCount = 0;
    }
}