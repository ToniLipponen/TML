#pragma once
#include <TML/System/Math/Vector2.h>
#include <vector>

namespace tml
{
    struct Ray
    {
        struct Intersection
        {
            struct Info
            {
                Vector2f position = {0,0};
                float distance = 0;
            };
            inline constexpr bool operator==(bool) const noexcept { return  intersects; }
            inline constexpr bool operator!=(bool) const noexcept { return !intersects; }
            inline Info& operator[](uint32_t index) noexcept { return points.at(index); }

            bool intersects = false;
            std::vector<Info> points;
        };

        Ray();
        Ray(const Vector2f& pos, const Vector2f& dir);

        Intersection IntersectsRect(const Vector2f& pos, const Vector2f& size, float rotation = 0.0f) const noexcept;
        Intersection IntersectsCircle(const Vector2f& pos, float radius) const noexcept;
        Intersection IntersectsLine(const Vector2f& a, const Vector2f& b) const noexcept;

        // Checks intersection between a ray and a shape made out of connected points.
        Intersection IntersectsShape(const std::vector<Vector2f>& points) const noexcept;

        Vector2f position;
        Vector2f direction;
    };
}