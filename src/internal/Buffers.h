#pragma once
#include "../../include/TML/Types.h"
#include <utility>
#include <vector>
#include <tuple>

namespace tml {
    class VertexBuffer {
    public:
        VertexBuffer();
        VertexBuffer(const void *data, ui32 vertexsize, ui32 numofvertices);
        ~VertexBuffer();
        void BufferData(void *data, ui32 vertexsize, ui32 numofvertices);
        void PushData(void *data, ui32 vertexsize, ui32 numofversices);
        void SetData(void *data, ui32 vertexsize, ui32 numofvertices);
        void Flush();

        [[maybe_unused]] constexpr ui32 DataSize() const {
            return m_datasize;
        }

        constexpr ui32 VertexCount() const {
            return m_vertex_count;
        }

    public:
        friend class VertexArray;

    protected:
        ui32 m_id, m_datasize, m_vertex_count;
    };

    class IndexBuffer {
    public:
        IndexBuffer();
        IndexBuffer(const ui32 *data, ui32 elements);
        ~IndexBuffer();
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
            VERTEX_BYTE = 0x1400,
            VERTEX_UNSIGNED_BYTE  = 0x1401,
            VERTEX_SHORT = 0x1402,
            VERTEX_UNSIGNED_SHORT = 0x1403,
            VERTEX_INT = 0x1404,
            VERTEX_UNSIGNED_INT = 0x1405,
            VERTEX_FLOAT = 0x1406,
        };
    public:
        BufferLayout() : m_stride(0) {}

        explicit BufferLayout(std::vector<std::tuple<ui32, ui32, DataType>> layout)
        : m_layout(std::move(layout)), m_stride(0) {}

        void Push(ui32 elements, ui32 size, DataType type) {
            m_layout.push_back(std::tuple<ui32,ui32,DataType>(elements, size, type));
            m_stride += elements * size;
        }

        void Clear() {
            m_layout.clear();
            m_stride = 0;
        }

        constexpr std::vector<std::tuple<ui32, ui32, DataType>> const &GetData() const {
            return m_layout;
        }

        constexpr ui32 GetStride() const {
            return m_stride;
        }

    private:
        //				 Elements | size
        std::vector<std::tuple<ui32, ui32, DataType>> m_layout;
        ui32 m_stride;
    };

    class VertexArray {
    public:
        VertexArray();

        VertexArray(VertexBuffer &vb, BufferLayout &layout);

        ~VertexArray();
        void BufferData(VertexBuffer &vb, BufferLayout &layout) noexcept;

        void BufferData(VertexBuffer &vb, IndexBuffer &ib, BufferLayout &layout) noexcept;

        void Bind() const noexcept;

        void Unbind() const noexcept;

        constexpr ui32 VertexCount() const noexcept {
            return m_vertex_count;
        }

    private:
        ui32 m_id, m_vertex_count;
    };
}