#pragma once
#include <TML/System/Math/Vector2.h>
#include <cmath>
#include <random>
#include <type_traits>

namespace tml::Math
{
    inline constexpr double Pi = 3.14159265358979323846;
    inline constexpr double Pi2 = Pi / 2;

    namespace Impl
    {
        inline static std::random_device randomDevice;
        inline static std::mt19937 generator(randomDevice());
    }

    /** @brief Converts from degrees to radians. */
    inline constexpr double DegToRad(double x) noexcept
    {
        return x * 0.01745329252;
    }

    /** @brief Converts from radians to degrees. */
    inline constexpr double RadToDeg(double x) noexcept
    {
        return x * 57.295779513;
    }

    /** @brief rotate point around origin using cosine and sine of rotation angle.
     *  @param origin The point around which the rotation occurs.
     *  @param p The point you wish to rotate.
     *  @param r Angle of rotation in degrees. */
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

    /** @brief rotate point around origin using cosine and sine of rotation angle.
     *  @param origin The point around which the rotation occurs.
     *  @param p The point you wish to rotate.
     *  @param cos_r Cosine of rotation angle.
     *  @param sin_r Sine of rotation angle. */
    template<typename T>
    inline constexpr Vector2<T> Rotate(const Vector2<T>& origin, const Vector2<T>& point, float cos_r, float sin_r) noexcept
    {
        const auto p = point - origin;

        return {origin.x + p.x * cos_r - p.y * sin_r,
                origin.y + p.x * sin_r + p.y * cos_r};
    }

    /** @brief Returns the bigger of the two values. */
    template<typename T>
    inline constexpr T Max(T a, T b) noexcept
    {
        return (b < a) ? a : b;
    }

    /** @brief Returns the smaller of the two values. */
    template<typename T>
    inline constexpr T Min(T a, T b) noexcept
    {
        return (b > a) ? a : b;
    }

    /** @brief Returns value clamped between min and max. */
    template<typename T>
    inline constexpr T Clamp(T value, T min, T max) noexcept
    {
        return Min(Max(value, min), max);
    }

    /** @brief Returns 1 if x is more than edge, otherwise returns 0. */
    inline constexpr double Step(double edge, double x) noexcept
    {
        return static_cast<double>(x > edge);
    }

    inline constexpr double SmoothStep(double edge0, double edge1, double x) noexcept
    {
        const auto t = Clamp<double>((x - edge0) / (edge1 - edge0), 0.0, 1.0);
        return t * t * (3.0 - 2.0 * t);
    }

    template<typename T>
    inline constexpr T Lerp(const T& a, const T& b, double m) noexcept
    {
        return a * (1.0 - m) + b * m;
    }

    template<typename T>
    inline constexpr T Map(T value, T min1, T max1, T min2, T max2) noexcept
    {
        return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
    }

    template<typename T>
    inline constexpr T Quadratic(const T &p0, const T &p1, const T &p2, const double t) noexcept
    {
        return Lerp<T>(
                Lerp<T>(p0, p1, t),
                Lerp<T>(p1, p2, t),
                t);
    }

    template<typename T>
    inline constexpr T Cubic(const T &p0, const T &p1, const T &p2, const T &p3, const double t) noexcept
    {
        return Lerp<T>(
                Quadratic(p0, p1, p2, t),
                Quadratic(p1, p2, p3, t),
                t);
    }

    /** @brief Returns the distance between two points. */
    template<typename T>
    inline double Distance(const Vector2<T>& a, const Vector2<T>& b) noexcept
    {
        return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
    }

    /** @brief Turns an angle in degrees to a direction vector. */
    inline Vector2f AngleToHeading(double degrees) noexcept
    {
        return Vector2f(std::cos(DegToRad(degrees)), std::sin(DegToRad(degrees))).Normalized();
    }

    /** @brief Converts a direction vector into an angle.
     *  @param v Direction vector.
     *  @return Angle in radians. */
    template<typename T>
    inline double HeadingToAngle(const Vector2<T>& v) noexcept
    {
        return -Pi2 - atan2(static_cast<double>(v.x), static_cast<double>(v.y));
    }

    /// @brief Checks whether a value is withing given range
    template<typename T>
    inline constexpr bool InRange(const T& value, const T& min, const T& max) noexcept
    {
        return (value >= min && value <= max);
    }

    /** @brief Checks whether a point is withing a rectangle.
     *  @param point The point. Duh.
     *  @param pos The position of the rectangle.
     *  @param size The size of the rectangle.
     *  @param rotation The rotation of the rectangle. */
    inline bool PointInRect(const Vector2f& point, const Vector2f& pos, const Vector2f& size, float rotation) noexcept
    {
        const auto rp = Rotate(pos + (size * 0.5), point, -rotation);
        return (InRange(rp.x, pos.x, pos.x + size.x) && InRange(rp.y, pos.y, pos.y + size.y));
    }

    /** @brief Checks whether a point is withing a rectangle.
     *  @param point The point. Duh.
     *  @param pos The position of the rectangle.
     *  @param size The size of the rectangle.
     *  @param rotation The rotation of the rectangle.
     *  @param origin The origin point for the rotation. (relative to the position of the rectangle) */
    inline bool PointInRect(const Vector2f& point, const Vector2f& pos, const Vector2f& size, float rotation, const Vector2f& origin) noexcept
    {
        const auto rp = Rotate(pos + origin, point, -rotation);
        return (InRange(rp.x, pos.x, pos.x + size.x) && InRange(rp.y, pos.y, pos.y + size.y));
    }

    /** @brief Checks whether a point is within a circle.
     *  @param point The point.
     *  @param pos The center of the circle.
     *  @param radius The radius of the circle. */
    inline bool PointInCircle(const Vector2f& point, const Vector2f& pos, float radius) noexcept
    {
        return (Distance(point, pos) <= radius);
    }

    /** @brief Normalizes a vector. */
    template<typename T>
    inline constexpr Vector2<T> Normalize(const Vector2<T>& v) noexcept
    {
        return v.Normalized();
    }

    /**
     * Generates a random value within a given range.
     *
     * @tparam T Arithmetic value type
     * @param min Minimum value
     * @param max Maximum value
     * @return Random value between minimum and maximum
     */
    template<typename T>
    inline typename std::enable_if<std::is_arithmetic<T>::value, T>::type Random(T min, T max) noexcept
    {
        if constexpr(std::is_floating_point<T>::value)
        {
            std::uniform_real_distribution<T> distribution(min, max);

            return distribution(Impl::generator);
        }
        else if constexpr(std::is_integral<T>::value)
        {
            std::uniform_int_distribution<T> distribution(min, max);

            return distribution(Impl::generator);
        }
    }

    inline uint32_t Random() noexcept
    {
        return Impl::randomDevice();
    }
}