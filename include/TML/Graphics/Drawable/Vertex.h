#pragma once
#include <TML/System/Math/Vector2.h>

namespace tml
{
    struct Vertex
    {
        enum DrawableType : uint32_t
        {
            COLOR   = 0,
            TEXTURE = 1,
            TEXT    = 2,
            CIRCLE  = 3,
        };

        Vertex(const Vector2f& pos,
               const Vector2f& uv,
               uint32_t color,
               DrawableType type,
               uint32_t texture)
        {
            this->pos = pos;
            this->uv = uv;
            this->color = color;
            this->typeAndTexture = (type << 24) | (texture << 16);
        }

        Vector2f pos{};
        Vector2f uv{};
        uint32_t color{};
        uint32_t typeAndTexture{};
    };
}