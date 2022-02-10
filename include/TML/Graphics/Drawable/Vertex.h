#pragma once
#include "TML/Types.h"

namespace tml
{
    struct Vertex
    {
        enum DrawableType : ui32 { COLOR = 0, TEXTURE = 1, TEXT = 2, VIDEO = 3};
        Vector2f pos;
        Vector2f uv;
        ui32 color;
        ui32 tex; // TODO: Merge tex & type into one ui32 to save 4 bytes
        DrawableType type;
    };
}