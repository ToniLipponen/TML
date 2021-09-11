#pragma once
#include "../Types.h"

namespace tml {
    struct Vertex {
        enum Drawable_Type{
            CIRCLE, RECTANGLE, TEXTURE, TEXT, VIDEO
        };
        Vector2 pos;
        ui32 color;
        Vector2 uv;
        ui32 tex; // Merge tex & type into one ui32
        ui32 type;
    };
}