#pragma once
#include <TML/Types.h>
#include <utility>
#include <vector>
#include <tuple>

namespace tml {
    class VertexBuffer {
    public:
        VertexBuffer();
        VertexBuffer(const void *data, ui32 vertexSize, ui32 numOfVertices);
        ~VertexBuffer();
        void Bind() const; // Doesn't do anything on GL 4.5
        void Unbind() const;
        void BufferData(void *data, ui32 vertexSize, ui32 numOfVertices);
        void PushData(void *data, ui32 vertexSize, ui32 numOfVertices);
        void SetData(void *data, ui32 vertexSize, ui32 numOfVertices);
        void Flush();

        constexpr ui32 DataSize() const {
            return m_dataSize;
        }

        constexpr ui32 VertexCount() const {
            return m_vertexCount;
        }

    public:
        friend class VertexArray;

    protected:
        ui32 m_id, m_dataSize, m_vertexCount;
    };

    class IndexBuffer {
    public:
        IndexBuffer();
        IndexBuffer(const ui32 *data, ui32 elements);
        ~IndexBuffer();
        void Bind() const; // Doesn't do anything on GL 4.5
        void Unbind() const;
        void BufferData(const ui32 *data, ui32 elements);
        void PushData(const ui32 *data, ui32 elements);
        void SetData(const ui32 *data, ui32 elements);
        void Flush();
        constexpr ui32 Elements() const noexcept {
            return m_elements;
        }

    public:
        friend class VertexArray;

    private:
        ui32 m_id;
        mutable ui32 m_elements;
    };

    class BufferLayout {
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

        explicit BufferLayout(std::vector<Attribute> layout)
        : m_layout(std::move(layout)), m_stride(0) {}

        void Push(ui32 elements, ui32 size, DataType type) {
            m_layout.push_back(Attribute{elements, size, type});
            m_stride += elements * size;
        }

        void Clear() {
            m_layout.clear();
            m_stride = 0;
        }

        constexpr std::vector<Attribute> const &GetData() const {
            return m_layout;
        }

        constexpr ui32 GetStride() const {
            return m_stride;
        }

    private:
        //				  Elements | size | GL_TYPE
        std::vector<Attribute> m_layout;
        ui32 m_stride;
    };

    class VertexArray {
    public:
        VertexArray();
        VertexArray(VertexBuffer &vb, BufferLayout &layout);
        ~VertexArray();
        void Bind() const noexcept;
        void Unbind() const noexcept;
        void BufferData(VertexBuffer &vb, BufferLayout &layout) noexcept;
        void BufferData(VertexBuffer &vb, IndexBuffer &ib, BufferLayout &layout) noexcept;
        constexpr ui32 VertexCount() const noexcept {
            return m_vertexCount;
        }

    private:
        ui32 m_id, m_vertexCount;
    };
}