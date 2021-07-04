#pragma once
#include "../Types.h"


enum Drawable_Type{ CIRCLE, RECTANGLE, TEXTURE, TEXT };

struct Vertex
{
    Vector2 pos;
    Color color;
    Vector2 uv;
    ui32 tex;
    float rotation;
    ui32 type;
};