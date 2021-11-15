#pragma once
#include "Copy.h"
#include "Condition.h"
#include "String.h"
#include "Vector2.h"

namespace tml
{
    namespace Util
    {

        inline Vector2 Rotate(const Vector2 &origin, Vector2 p, float r)
        {
            r = (r * 0.01745329f);
            p -= origin;
            const float cos_r = cosf(r);
            const float sin_r = sinf(r);
            return {origin.x + p.x * cos_r - p.y * sin_r,
                    origin.y + p.x * sin_r + p.y * cos_r};
        }

        template<typename T>
        inline constexpr T Clamp(T value, T min, T max) noexcept
        {
            if (value > max)
                return max;
            if (value < min)
                return min;
            return value;
        }

        template<typename T>
        inline constexpr T Min(T value, T min) noexcept
        {
            if (value < min)
                return min;
            return value;
        }

        template<typename T>
        inline constexpr T Max(T value, T max) noexcept
        {
            if (value > max)
                return max;
            return value;
        }

        // Need types with +, - and * operators
        // T operator+(const T& rhs)
        // T operator-(const T& rhs)
        // T operator*(float rhs)
        template<typename T>
        inline constexpr T Lerp(const T &a, const T &b, float m) noexcept
        {
            return a + ((b - a) * m);
        }

        template<typename T>
        inline constexpr T Quadratic(const T &p0, const T &p1, const T &p2, const float t) noexcept
        {
            return Lerp(
                    Lerp(p0, p1, t),
                    Lerp(p1, p2, t),
                    t);
        }

        template<typename T>
        inline constexpr T Cubic(const T &p0, const T &p1, const T &p2, const T &p3, const float t) noexcept
        {
            return Lerp(
                    Quadratic(p0, p1, p2, t),
                    Quadratic(p1, p2, p3, t),
                    t);
        }

        // @brief Checks whether a value is withing given range
        template <typename T>
        inline constexpr bool InRange(const T& min, const T& value, const T& max) noexcept
        {
            if(value >= min && value <= max)
                return true;
            return false;
        }

        inline bool PointInRect(const Vector2& point, const Vector2& p, const Vector2& s, float rotation)
        {
            const auto npoint = Rotate(p + (s*0.5), point, -rotation);
            return (p < npoint && npoint < p+s);
        }

        inline bool PointInCircle(const Vector2& point, const Vector2& p, float r)
        {
            return (Vector2::Distance(point, p) <= r);
        }

        inline Vector2 AngleToHeading(float degrees) noexcept
        {
            return Vector2(std::cos(degrees * 0.01745329f), std::sin(degrees * 0.01745329f)).Normalized();
        }
    }
}