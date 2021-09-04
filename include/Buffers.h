#pragma once
#include "Types.h"
#include <utility>
#include <vector>

namespace tml {
    class VertexBuffer {
    public:
        VertexBuffer();

        // VertexBuffer(VertexBuffer&) = delete;
        // VertexBuffer(VertexBuffer&&) = delete;
        VertexBuffer(const void *data, ui32 vertexsize, ui32 numofvertices);

        ~VertexBuffer();

        void Bind() const noexcept;

        void BufferData(void *data, ui32 vertexsize, ui32 numofvertices);

        void PushData(void *data, ui32 vertexsize, ui32 numofversices);

        void PushData(void *data, ui32 bytes);

        void SetData(void *data, ui32 vertexsize, ui32 numofvertices);

        void Flush();

        constexpr ui32 DataSize() const {
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

        void Bind() const noexcept;

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
        BufferLayout() : m_stride(0) {}

        BufferLayout(const std::vector<std::pair<ui32, ui32>> &layout) : m_layout(layout), m_stride(0) {}

        void Push(ui32 elements, ui32 size) {
            m_layout.push_back({elements, size});
            m_stride += elements * size;
        }

        void Clear() {
            m_layout.clear();
            m_stride = 0;
        }

        constexpr std::vector<std::pair<ui32, ui32>> const &GetData() const {
            return m_layout;
        }

        constexpr ui32 GetStride() const {
            return m_stride;
        }

    private:
        //				 Elements | size
        std::vector<std::pair<ui32, ui32>> m_layout;
        ui32 m_stride;
    };

    class VertexArray {
    public:
        VertexArray();

        VertexArray(VertexBuffer &vb, BufferLayout &layout);

        // ~VertexArray();
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
};