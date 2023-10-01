#include <TML/Graphics/Core/BufferLayout.h>

namespace tml
{
    BufferLayout::BufferLayout()
    : m_stride(0)
    {

    }

    void BufferLayout::Push(uint32_t elements, uint32_t size, DataType type) noexcept
    {
        m_layout.push_back(Attribute{elements, size, type});
        m_stride += elements * size;
    }

    void BufferLayout::Clear() noexcept
    {
        m_layout.clear();
        m_stride = 0;
    }

    const std::vector<BufferLayout::Attribute>& BufferLayout::GetData() const noexcept 
    { 
        return m_layout; 
    }

    uint32_t BufferLayout::GetStride() const noexcept 
    { 
        return m_stride; 
    }
}