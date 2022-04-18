#pragma once
#include <cstring>

namespace tml
{
    VertexBuffer::VertexBuffer() noexcept
    : m_id(0), m_dataSize(0), m_vertexCount(0), m_capacity(0)
    {
        GL_CALL(glad_glCreateBuffers(1, &m_id));
    }

    VertexBuffer::VertexBuffer(const void* data, uint32_t vertexSize, uint32_t vertexCount) noexcept
    : m_id(0), m_dataSize(0), m_vertexCount(0), m_capacity(vertexSize * vertexCount)
    {
        GL_CALL(glad_glCreateBuffers(1, &m_id));
        BufferData(data, vertexSize, vertexCount);
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

    void VertexBuffer::BufferData(const void* data, uint32_t vertexSize, uint32_t vertexCount) noexcept
    {
        if(m_mappedPtr)
        {
            GL_CALL(glad_glFlushMappedNamedBufferRange(m_id, 0, m_dataSize));
            GL_CALL(glad_glUnmapNamedBuffer(m_id));
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

        GL_CALL(glad_glNamedBufferData(m_id, m_capacity, data, GL_STATIC_DRAW));
        m_mappedPtr = GL_CALL(glad_glMapNamedBufferRange(m_id, 0, m_capacity, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT));
    }

    void VertexBuffer::PushData(const void* data, uint32_t vertexSize, uint32_t vertexCount) noexcept
    {
        if(m_mappedPtr)
        {
            const uint32_t size = vertexSize * vertexCount;
            auto dest = ((uint8_t*) m_mappedPtr) + m_dataSize;
            std::memcpy(dest, data, size);
            m_dataSize += size;
            m_vertexCount += vertexCount;
        }
    }
}