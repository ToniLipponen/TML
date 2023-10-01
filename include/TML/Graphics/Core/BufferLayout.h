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
        BufferLayout();

        void Push(uint32_t elements, uint32_t size, DataType type) noexcept;

        void Clear() noexcept;

        [[nodiscard]]
        std::vector<Attribute> const & GetData() const noexcept;

        [[nodiscard]]
        uint32_t GetStride() const noexcept;

    private:
        std::vector<Attribute> m_layout;
        uint32_t m_stride;
    };
}