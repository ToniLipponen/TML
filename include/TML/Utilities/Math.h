#pragma once
#include <TML/Graphics/Vector2.h>
#include <cmath>
#define DEG2RAD(x) (x * 0.01745329f)

namespace tml
{
    namespace Math
    {
        template<typename T>
        inline Vector2<T> Rotate(const Vector2<T> &origin, Vector2<T> p, double r)
        {
            r = DEG2RAD(r);
            p -= origin;
            const double cos_r = cos(r);
            const double sin_r = sin(r);
            return {origin.x + p.x * cos_r - p.y * sin_r,
                    origin.y + p.x * sin_r + p.y * cos_r};
        }

        template<typename T>
        inline constexpr T Clamp(T value, T min, T max) noexcept
        {
            if(value > max)
                return max;
            if(value < min)
                return min;
            return value;
        }

        template<typename T>
        inline constexpr T Min(T value, T min) noexcept
        {
            if(value < min)
                return min;
            return value;
        }

        template<typename T>
        inline constexpr T Max(T value, T max) noexcept
        {
            if(value > max)
                return max;
            return value;
        }

        template<typename T>
        inline constexpr T Step(T edge, T x) noexcept
        {
            if(x < edge)
                return 0;
            return 1;
        }

        template<typename T>
        inline constexpr T SmoothStep(T edge0, T edge1, T x) noexcept
        {
            constexpr const T t = Clamp<T>((x - edge0) / (edge1 - edge0), 0.0, 1.0);
            return t * t * (3.0 - 2.0 * t);
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

        inline double Distance(const Vector2f &a, const Vector2f &b) noexcept
        {
            return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
        }

        inline Vector2f AngleToHeading(float degrees) noexcept
        {
            return Vector2f(std::cos(degrees * 0.01745329f), std::sin(degrees * 0.01745329f)).Normalized();
        }

        // @brief Checks whether a value is withing given range
        template <typename T>
        inline constexpr bool InRange(const T& min, const T& value, const T& max) noexcept
        {
            if(value >= min && value <= max)
                return true;
            return false;
        }
    }
}