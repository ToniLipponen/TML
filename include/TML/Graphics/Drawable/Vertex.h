#pragma once
#include <TML/System/Math/Vector2.h>

namespace tml
{
    struct Vertex
    {
        enum DrawableType : uint32_t { COLOR = 0, TEXTURE = (1<<16), TEXT = (2 << 16), VIDEO = (3 << 16) };
        Vector2f pos{};
        Vector2f uv{}; //!< Maybe use Vector2<short> for uv. This would limit the range of values and lose some precision. But would save another 4 bytes.
        uint32_t color = 0;
        uint32_t texAndType = 0;
    };
}