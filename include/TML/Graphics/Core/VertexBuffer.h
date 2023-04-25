#pragma once
#include <TML/Export.h>

#include <cstdint>

namespace tml
{
    class TML_API VertexBuffer
    {
    public:
        VertexBuffer() noexcept;
        VertexBuffer(const void *data, uint32_t vertexSize, uint32_t numOfVertices) noexcept;
        ~VertexBuffer() noexcept;
        void Bind() noexcept;
        void Unbind() const noexcept;
        void BufferData(const void *data, uint32_t vertexSize, uint32_t numOfVertices) noexcept;
        void PushData(const void *data, uint32_t vertexSize, uint32_t numOfVertices) noexcept;
        uint32_t DataSize() const noexcept { return m_dataSize; }
        uint32_t VertexCount() const noexcept { return m_vertexCount; }

    public:
        friend class VertexArray;

    private:
        uint32_t m_id;

    protected:
        uint32_t m_dataSize, m_vertexCount, m_capacity;
    };
}