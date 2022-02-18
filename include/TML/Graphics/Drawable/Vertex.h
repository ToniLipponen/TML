#pragma once
#include "TML/Types.h"

namespace tml
{
    struct Vertex
    {
        enum DrawableType : ui32 { COLOR = 0, TEXTURE = (1<<16), TEXT = (2 << 16), VIDEO = (3 << 16) };
        Vector2f pos{};
        Vector2f uv{}; // Maybe use Vector2<short> for uv. This would limit the range of values and lose some precision. But would save another 4 bytes.
        ui32 color = 0;
        ui32 texAndType = 0;
    };
}