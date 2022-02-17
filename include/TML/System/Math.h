#pragma once
#include "TML/Graphics/Vector2.h"
#include <cmath>

namespace tml
{
    namespace Math
    {
        /// @brief Converts from degrees to radians.
        inline constexpr double DegToRad(double x) noexcept
        {
            return x * 0.01745329252;
        }

        /// @brief Converts from radians to degrees.
        inline constexpr double RadToDeg(double x) noexcept
        {
            return x * 57.295779513;
        }

        template<typename T>
        inline Vector2<T> Rotate(const Vector2<T> &origin, Vector2<T> p, double r) noexcept
        {
            r = DegToRad(r);
            p -= origin;
            const double cos_r = std::cos(r);
            const double sin_r = std::sin(r);
            return {origin.x + p.x * cos_r - p.y * sin_r,
                    origin.y + p.x * sin_r + p.y * cos_r};
        }

        template<typename T>
        inline constexpr Vector2<T> Rotate(const Vector2<T>& origin, Vector2<T> p, float cos_r, float sin_r) noexcept
        {
            p -= origin;
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

        /// Returns the bigger of the two values
        template<typename T>
        inline constexpr T Max(T a, T b) noexcept
        {
            if(b < a)
                return a;
            return b;
        }

        /// Returns the smaller of the two values
        template<typename T>
        inline constexpr T Min(T a, T b) noexcept
        {
            if(b > a)
                return a;
            return b;
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

        /// Linear interpolation
        template<typename T>
        inline constexpr T Lerp(const T &a, const T &b, double m) noexcept
        {
            return a + ((b - a) * m);
        }

        template<typename T>
        inline constexpr T Quadratic(const T &p0, const T &p1, const T &p2, const double t) noexcept
        {
            return Lerp(
                    Lerp(p0, p1, t),
                    Lerp(p1, p2, t),
                    t);
        }

        template<typename T>
        inline constexpr T Cubic(const T &p0, const T &p1, const T &p2, const T &p3, const double t) noexcept
        {
            return Lerp(
                    Quadratic(p0, p1, p2, t),
                    Quadratic(p1, p2, p3, t),
                    t);
        }

        template<typename T>
        inline double Distance(const Vector2<T>& a, const Vector2<T>& b) noexcept
        {
            return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
        }

        inline Vector2f AngleToHeading(double degrees) noexcept
        {
            return Vector2f(std::cos(DegToRad(degrees)), std::sin(DegToRad(degrees))).Normalized();
        }

        /// Checks whether a value is withing given range
        template<typename T>
        inline constexpr bool InRange(const T& value, const T& min, const T& max) noexcept
        {
            return (value >= min && value <= max);
        }

        inline bool PointInRect(const Vector2f& point, const Vector2f& p, const Vector2f& s, float rotation) noexcept
        {
            const auto rp = Rotate(p + (s * 0.5), point, -rotation);
            return (InRange(rp.x, p.x, p.x + s.x) && InRange(rp.y, p.y, p.y + s.y));
        }

        inline bool PointInCircle(const Vector2f& point, const Vector2f& p, float r) noexcept
        {
            return (Distance(point, p) <= r);
        }
    }
}