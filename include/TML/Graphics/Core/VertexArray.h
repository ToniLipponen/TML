#pragma once
#include <TML/Graphics/Core/VertexBuffer.h>
#include <TML/Graphics/Core/BufferLayout.h>
#include <TML/Graphics/Core/IndexBuffer.h>

#include <cstdint>

namespace tml
{
    class TML_API VertexArray
    {
    public:
        VertexArray() noexcept;
        VertexArray(VertexBuffer &vb, BufferLayout &layout) noexcept;
        ~VertexArray() noexcept;
        void Bind() const noexcept;
        void Unbind() const noexcept;
        void BufferData(VertexBuffer &vb, BufferLayout &layout) noexcept;
        void BufferData(VertexBuffer &vb, IndexBuffer &ib, BufferLayout &layout) noexcept;
        uint32_t VertexCount() const noexcept { return m_vertexCount; }

    private:
        uint32_t m_id, m_vertexCount;
    };
}