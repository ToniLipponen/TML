#pragma once
#include <TML/Export.h>

#include <cstdint>
#include <vector>

namespace tml
{
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
}