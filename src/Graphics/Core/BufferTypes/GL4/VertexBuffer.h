#pragma once
#include <TML/Graphics/Core/Buffers.h>
#include <cstring>
#include "../BufferFlags.h"
#include "../../../Headers/GLHeader.h"

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
        m_id = 0;
    }

    void VertexBuffer::Bind() noexcept
    {

    }

    void VertexBuffer::Unbind() const noexcept
    {

    }

    void VertexBuffer::BufferData(const void* data, uint32_t vertexSize, uint32_t vertexCount) noexcept
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

        GL_CALL(glad_glNamedBufferData(m_id, m_capacity, data, BUFFER_USAGE_FLAG));
    }

    void VertexBuffer::PushData(const void* data, uint32_t vertexSize, uint32_t vertexCount) noexcept
    {
        const uint32_t size = vertexSize * vertexCount;
        GL_CALL(glad_glNamedBufferSubData(m_id, m_dataSize, size, data));
        m_dataSize += size;
        m_vertexCount += vertexCount;
    }
}