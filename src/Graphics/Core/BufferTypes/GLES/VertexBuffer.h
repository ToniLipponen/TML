#pragma once
#include <cstring>

namespace tml
{
    VertexBuffer::VertexBuffer() noexcept
    : m_id(0), m_dataSize(0), m_vertexCount(0)
    {
        GL_CALL(glad_glGenBuffers(1, &m_id));
    }

    VertexBuffer::VertexBuffer(const void* data, ui32 vertexSize, ui32 vertexCount) noexcept
    : m_id(0), m_dataSize(0), m_vertexCount(0), m_capacity(vertexSize * vertexCount)
    {
        GL_CALL(glad_glGenBuffers(1, &m_id));
        BufferData(data, vertexSize, vertexCount);
    }

    VertexBuffer::~VertexBuffer() noexcept
    {
        GL_CALL(glad_glDeleteBuffers(1, &m_id));
    }

    void VertexBuffer::Bind() const noexcept
    {
        GL_CALL(glad_glBindBuffer(GL_ARRAY_BUFFER, m_id));
    }

    void VertexBuffer::Unbind() const noexcept
    {
        GL_CALL(glad_glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

    void VertexBuffer::BufferData(const void* data, ui32 vertexSize, ui32 vertexCount) noexcept
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

        Bind();

        if(m_mappedPtr)
        {
            GL_CALL(glad_glUnmapBuffer(GL_ARRAY_BUFFER));
        }

        GL_CALL(glad_glBufferData(GL_ARRAY_BUFFER, m_capacity, data, GL_DYNAMIC_DRAW));
        m_mappedPtr = GL_CALL(glad_glMapBufferRange(GL_ARRAY_BUFFER, 0, m_capacity, GL_MAP_WRITE_BIT));

        Unbind();
    }

    void VertexBuffer::PushData(const void* data, ui32 vertexSize, ui32 vertexCount) noexcept
    {
        Bind();
        if(m_mappedPtr)
        {
            const ui32 size = vertexSize * vertexCount;
            auto dest = ((ui8 *) m_mappedPtr) + m_dataSize;
            std::memcpy(dest, data, size);
            m_dataSize += size;
            m_vertexCount += vertexCount;
        }
        Unbind();
    }
}