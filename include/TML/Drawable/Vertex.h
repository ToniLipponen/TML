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
        ui32 tex; // TODO: Merge tex & type into one ui32
        Drawable_Type type;
    };
}