#pragma once
#include <TML/Export.h>

#include <cstdint>

namespace tml
{
    class TML_API IndexBuffer
    {
    public:
        IndexBuffer() noexcept;
        IndexBuffer(const uint32_t *data, uint32_t elements) noexcept;
        ~IndexBuffer() noexcept;
        void Bind() noexcept;
        void Unbind() const noexcept;
        void BufferData(const uint32_t *data, uint32_t elements) noexcept;
        void PushData(const uint32_t *data, uint32_t elements) noexcept;
        uint32_t Elements() const noexcept { return m_elements; }

    public:
        friend class VertexArray;

    private:
        uint32_t m_id;
        
    protected:
        mutable uint32_t m_elements;
        uint32_t m_capacity;
    };
}