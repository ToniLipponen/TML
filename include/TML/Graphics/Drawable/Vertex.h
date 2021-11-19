#pragma once
#include "TML/Utilities/Types.h"

namespace tml
{
    struct Vertex
    {
        enum DrawableType : ui32 { CIRCLE = 0, RECTANGLE = 1, TEXTURE = 2, TEXT = 3, VIDEO = 4 };
        Vector2f pos;
        Vector2f uv;
        ui32 color;
        ui32 tex; // TODO: Merge tex & type into one ui32 to save 4 bytes
        DrawableType type;
    };
}