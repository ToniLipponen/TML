#pragma once
#include "TML/Types.h"
#include <utility>
#include <vector>

namespace tml
{
    class VertexBuffer
    {
    public:
        VertexBuffer() noexcept;
        VertexBuffer(const void *data, ui32 vertexSize, ui32 numOfVertices) noexcept;
        ~VertexBuffer() noexcept;
        void Bind() const noexcept;
        void Unbind() const noexcept;
        void BufferData(const void *data, ui32 vertexSize, ui32 numOfVertices) noexcept;
        void PushData(const void *data, ui32 vertexSize, ui32 numOfVertices) noexcept;
        constexpr ui32 DataSize() const noexcept { return m_dataSize; }
        constexpr ui32 VertexCount() const noexcept { return m_vertexCount; }

    public:
        friend class VertexArray;

    private:
        ui32 m_id;

    protected:
        ui32 m_dataSize, m_vertexCount, m_capacity;
        void* m_mappedPtr = nullptr;
    };

    class IndexBuffer
    {
    public:
        IndexBuffer() noexcept;
        IndexBuffer(const ui32 *data, ui32 elements) noexcept;
        ~IndexBuffer() noexcept;
        void Bind() const noexcept; // Doesn't do anything on GL 4.5
        void Unbind() const noexcept;
        void BufferData(const ui32 *data, ui32 elements) noexcept;
        void PushData(const ui32 *data, ui32 elements) noexcept;
        inline constexpr ui32 Elements() const noexcept { return m_elements; }

    public:
        friend class VertexArray;

    private:
        ui32 m_id;
        
    protected:
        mutable ui32 m_elements;
        ui32 m_capacity;
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
            ui32 elements, size;
            DataType dataType;
        };
    public:
        BufferLayout() : m_stride(0) {}

        void Push(ui32 elements, ui32 size, DataType type) noexcept
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
        inline constexpr ui32 GetStride() const noexcept { return m_stride; }

    private:
        std::vector<Attribute> m_layout;
        ui32 m_stride;
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
        constexpr ui32 VertexCount() const noexcept { return m_vertexCount; }

    private:
        ui32 m_id, m_vertexCount;
    };

    class StorageBuffer /// Shader Storage Buffer
    {
    public:
        StorageBuffer() noexcept;
        void Bind() const noexcept;
        void Unbind() const noexcept;
        void BufferData(const void* data, ui32 bytes) noexcept;
        void UpdateData(const void* data, ui32 bytes) noexcept;
        void RetrieveData(void* data, ui32 bytes) noexcept;
    protected:
        void BindBufferBase(ui32 index) const noexcept;
        friend class ComputeShader;
    private:
        ui32 m_id;
    };
}