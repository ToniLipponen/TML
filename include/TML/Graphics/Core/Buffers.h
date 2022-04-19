#pragma once
#include <cstdint>
#include <utility>
#include <vector>

namespace tml
{
    class VertexBuffer
    {
    public:
        VertexBuffer() noexcept;
        VertexBuffer(const void *data, uint32_t vertexSize, uint32_t numOfVertices) noexcept;
        ~VertexBuffer() noexcept;
        void Bind() noexcept;
        void Unbind() const noexcept;
        void BufferData(const void *data, uint32_t vertexSize, uint32_t numOfVertices) noexcept;
        void PushData(const void *data, uint32_t vertexSize, uint32_t numOfVertices) noexcept;
        constexpr uint32_t DataSize() const noexcept { return m_dataSize; }
        constexpr uint32_t VertexCount() const noexcept { return m_vertexCount; }

    public:
        friend class VertexArray;

    private:
        uint32_t m_id;

    protected:
        uint32_t m_dataSize, m_vertexCount, m_capacity;
        void* m_mappedPtr = nullptr;
    };

    class IndexBuffer
    {
    public:
        IndexBuffer() noexcept;
        IndexBuffer(const uint32_t *data, uint32_t elements) noexcept;
        ~IndexBuffer() noexcept;
        void Bind() noexcept;
        void Unbind() const noexcept;
        void BufferData(const uint32_t *data, uint32_t elements) noexcept;
        void PushData(const uint32_t *data, uint32_t elements) noexcept;
        inline constexpr uint32_t Elements() const noexcept { return m_elements; }

    public:
        friend class VertexArray;

    private:
        uint32_t m_id;
        
    protected:
        mutable uint32_t m_elements;
        uint32_t m_capacity;
        void* m_mappedPtr = nullptr;
    };

    class BufferLayout
    {
    public:
        enum DataType
        {
            VERTEX_BYTE             = 0x1400,
            VERTEX_UNSIGNED_BYTE    = 0x1401,
            VERTEX_SHORT            = 0x1402,
            VERTEX_UNSIGNED_SHORT   = 0x1403,
            VERTEX_INT              = 0x1404,
            VERTEX_UNSIGNED_INT     = 0x1405,
            VERTEX_FLOAT            = 0x1406,
        };
        struct Attribute
        {
            uint32_t elements, size;
            DataType dataType;
        };
    public:
        BufferLayout() : m_stride(0) {}

        void Push(uint32_t elements, uint32_t size, DataType type) noexcept
        {
            m_layout.push_back(Attribute{elements, size, type});
            m_stride += elements * size;
        }

        void Clear()
        {
            m_layout.clear();
            m_stride = 0;
        }

        inline constexpr std::vector<Attribute> const & GetData() const noexcept { return m_layout; }
        inline constexpr uint32_t GetStride() const noexcept { return m_stride; }

    private:
        std::vector<Attribute> m_layout;
        uint32_t m_stride;
    };

    class VertexArray
    {
    public:
        VertexArray() noexcept;
        VertexArray(VertexBuffer &vb, BufferLayout &layout) noexcept;
        ~VertexArray() noexcept;
        void Bind() const noexcept;
        void Unbind() const noexcept;
        void BufferData(VertexBuffer &vb, BufferLayout &layout) noexcept;
        void BufferData(VertexBuffer &vb, IndexBuffer &ib, BufferLayout &layout) noexcept;
        constexpr uint32_t VertexCount() const noexcept { return m_vertexCount; }

    private:
        uint32_t m_id, m_vertexCount;
    };

    class StorageBuffer /// Shader Storage Buffer
    {
    public:
        StorageBuffer() noexcept;
        void Bind() const noexcept;
        void Unbind() const noexcept;
        void BufferData(const void* data, uint32_t bytes) const noexcept;
        void UpdateData(const void* data, uint32_t bytes) const noexcept;
        void RetrieveData(void* data, uint32_t bytes) const noexcept;
    protected:
        void BindBufferBase(uint32_t index) const noexcept;
        friend class ComputeShader;
    private:
        uint32_t m_id;
    };
}