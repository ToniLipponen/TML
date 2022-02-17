#pragma once
#include "TML/Types.h"

namespace tml
{
    struct Vertex
    {
        enum DrawableType : ui32 { COLOR = 0, TEXTURE = (1<<16), TEXT = (2 << 16), VIDEO = (3 << 16)};
        Vector2f pos;
        Vector2f uv;
        ui32 color;
        ui32 texAndType;
    };
}