#pragma once
#include "../Types.h"


namespace tml {
// Might want to shrink this down at some point.
    struct Vertex {
        enum Drawable_Type {
            CIRCLE, RECTANGLE, TEXTURE, TEXT, VIDEO
        };
        Vector2 pos;
        Color color;
        Vector2 uv;
        ui32 tex;
        ui32 type;
    };
}