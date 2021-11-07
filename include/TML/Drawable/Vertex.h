#pragma once
#include "../Types.h"

namespace tml
{
    struct Vertex
    {
        enum DrawableType : ui32 { CIRCLE, RECTANGLE, TEXTURE, TEXT, VIDEO };
        Vector2 pos;
        Vector2 uv;
        ui32 color;
        ui32 tex; // TODO: Merge tex & type into one ui32 to save 4 bytes
        DrawableType type;
    };
}