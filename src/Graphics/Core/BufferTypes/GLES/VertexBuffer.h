#pragma once
#include <cstring>

namespace tml
{
    VertexBuffer::VertexBuffer() noexcept
    : m_id(0), m_dataSize(0), m_vertexCount(0)
    {
        GL_CALL(glad_glGenBuffers(1, &m_id));
    }

    VertexBuffer::VertexBuffer(const void* data, uint32_t vertexSize, uint32_t vertexCount) noexcept
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

    void VertexBuffer::BufferData(const void* data, uint32_t vertexSize, uint32_t vertexCount) noexcept
    {
        Bind();

        if(m_mappedPtr)
        {
            GL_CALL(glad_glFlushMappedBufferRange(GL_ARRAY_BUFFER, 0, m_dataSize));
            GL_CALL(glad_glUnmapBuffer(GL_ARRAY_BUFFER));
        }

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

        GL_CALL(glad_glBufferData(GL_ARRAY_BUFFER, m_capacity, data, BUFFER_USAGE_FLAG));
        m_mappedPtr = GL_CALL(glad_glMapBufferRange(GL_ARRAY_BUFFER, 0, m_capacity, BUFFER_MAP_FLAGS));

        Unbind();
    }

    void VertexBuffer::PushData(const void* data, uint32_t vertexSize, uint32_t vertexCount) noexcept
    {
        Bind();
        if(m_mappedPtr)
        {
            const uint32_t size = vertexSize * vertexCount;
            auto dest = ((uint8_t*) m_mappedPtr) + m_dataSize;
            std::memcpy(dest, data, size);
            m_dataSize += size;
            m_vertexCount += vertexCount;
        }
        Unbind();
    }
}