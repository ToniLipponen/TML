#pragma once
#include <TML/System/Math/Vector2.h>
#include <random>
#include <type_traits>
#include <limits>
#include <iterator>

namespace tml::Math
{
    [[maybe_unused]] inline constexpr double Pi = 3.14159265358979323846;
    [[maybe_unused]] inline constexpr double Pi2 = Pi / 2;

    namespace Impl
    {
        inline static std::random_device randomDevice;
        inline static std::mt19937 randomGenerator(randomDevice());
    }

    /** @brief Converts from degrees to radians. */
    template<typename T>
    [[nodiscard]]
    inline constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type DegreesToRadians(T x) noexcept
    {
        return x * 0.01745329252;
    }

    /** @brief Converts from radians to degrees. */
    template<typename T>
    [[nodiscard]]
    inline constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type RadiansToDegrees(T x) noexcept
    {
        return x * 57.295779513;
    }

    /** @brief rotate point around origin using cosine and sine of rotation angle.
     *  @param origin The point around which the rotation occurs.
     *  @param p The point you wish to rotate.
     *  @param r Angle of rotation in degrees. */
    template<typename T>
    [[nodiscard]]
    inline Vector2<T> Rotate(const Vector2<T>& origin, Vector2<T> p, float r) noexcept
    {
        r = static_cast<float>(DegreesToRadians(r));
        p = p - origin;
        const double cos_r = std::cos(r);
        const double sin_r = std::sin(r);
        return Vector2<T>{origin.x + p.x * cos_r - p.y * sin_r,
                origin.y + p.x * sin_r + p.y * cos_r};
    }

    /** @brief rotate point around origin using cosine and sine of rotation angle.
     *  @param origin The point around which the rotation occurs.
     *  @param p The point you wish to rotate.
     *  @param cos_r Cosine of rotation angle.
     *  @param sin_r Sine of rotation angle. */
    template<typename T>
    [[nodiscard]]
    inline constexpr Vector2<T> Rotate(const Vector2<T>& origin, const Vector2<T>& point, float cos_r, float sin_r) noexcept
    {
        const auto p = point - origin;

        return {origin.x + p.x * cos_r - p.y * sin_r,
                origin.y + p.x * sin_r + p.y * cos_r};
    }

    template<typename T>
    [[nodiscard]]
    inline constexpr Vector2<T> Rotate(const Vector2<T>& point, float r) noexcept
    {
        const float cos_r = std::cos(r);
        const float sin_r = std::sin(r);
        return {point.x * cos_r - point.y * sin_r,
                point.x * sin_r + point.y * cos_r};
    }

    template<typename T>
    [[nodiscard]]
    inline constexpr Vector2<T> Rotate(const Vector2<T>& point, float cos_r, float sin_r) noexcept
    {
        return {point.x * cos_r - point.y * sin_r,
                point.x * sin_r + point.y * cos_r};
    }

    /** @brief Returns the bigger of the two values. */
    template<typename T>
    [[nodiscard]]
    inline constexpr T Max(T a, T b) noexcept
    {
        return (b < a) ? a : b;
    }

    /** @brief Returns the smaller of the two values. */
    template<typename T>
    [[nodiscard]]
    inline constexpr T Min(T a, T b) noexcept
    {
        return (b > a) ? a : b;
    }

    /** @brief Returns value clamped between min and max. */
    template<typename T>
    [[nodiscard]]
    inline constexpr T Clamp(T value, T min, T max) noexcept
    {
        return Min(Max(value, min), max);
    }

    /** @brief Returns 1 if x is more than edge, otherwise returns 0. */
    template<typename T>
    inline constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type Step(T edge, T x) noexcept
    {
        return static_cast<T>(x > edge);
    }

    template<typename T>
    [[nodiscard]]
    inline constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type SmoothStep(T edge0, T edge1, T x) noexcept
    {
        const auto t = Clamp<T>((x - edge0) / (edge1 - edge0), 0.0, 1.0);
        return t * t * (3.0 - 2.0 * t);
    }

    /** @brief Linear interpolation. */
    template<typename T>
    [[nodiscard]]
    inline constexpr T Lerp(const T& a, const T& b, double m) noexcept
    {
        return a * (1.0 - m) + b * m;
    }

    /** @brief Maps a value from one range to another range. */
    template<typename T>
    [[nodiscard]]
    inline constexpr T Map(T value, T min1, T max1, T min2, T max2) noexcept
    {
        return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
    }

    template<typename T>
    [[nodiscard]]
    inline constexpr T Quadratic(const T &p0, const T &p1, const T &p2, const double t) noexcept
    {
        return Lerp<T>(
                Lerp<T>(p0, p1, t),
                Lerp<T>(p1, p2, t),
                t);
    }

    template<typename T>
    [[nodiscard]]
    inline constexpr T Cubic(const T &p0, const T &p1, const T &p2, const T &p3, const double t) noexcept
    {
        return Lerp<T>(
                Quadratic(p0, p1, p2, t),
                Quadratic(p1, p2, p3, t),
                t);
    }

    /** @brief Returns the distance between two points. */
    template<typename T>
    [[nodiscard]]
    inline double Distance(const Vector2<T>& a, const Vector2<T>& b) noexcept
    {
        return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
    }

    /** @brief Turns an angle in degrees to a direction vector. */
    template<typename T, typename R = float>
    [[nodiscard]]
	inline typename std::enable_if<std::is_floating_point<T>::value, Vector2<R>>::type AngleToHeading(T degrees) noexcept
    {
        return Vector2<R>(std::cos(DegreesToRadians(degrees)), std::sin(DegreesToRadians(degrees))).Normalized();
    }

    /** @brief Converts a direction vector into an angle.
     *  @param v Direction vector.
     *  @return Angle in radians. */
    template<typename T>
    [[nodiscard]]
    inline double HeadingToAngle(const Vector2<T>& v) noexcept
    {
        return -Pi2 - atan2(static_cast<double>(v.x), static_cast<double>(v.y));
    }

    /** @brief Checks whether a value is withing given range. */
    template<typename T>
    [[nodiscard]]
    inline constexpr bool InRange(const T& value, const T& min, const T& max) noexcept
    {
        return (value >= min && value <= max);
    }

    /** @brief Checks whether a point is withing a rectangle.
     *  @param point The point. Duh.
     *  @param pos The position of the rectangle.
     *  @param size The size of the rectangle.
     *  @param rotation The rotation of the rectangle. */
    [[nodiscard]]
    inline bool PointInRect(const Vector2f& point, const Vector2f& pos, const Vector2f& size, float rotation) noexcept
    {
        const Vector2f rp = Rotate(pos + (size * 0.5f), point, -rotation);
        return (InRange(rp.x, pos.x, pos.x + size.x) && InRange(rp.y, pos.y, pos.y + size.y));
    }

    /** @brief Checks whether a point is withing a rectangle.
     *  @param point The point. Duh.
     *  @param pos The position of the rectangle.
     *  @param size The size of the rectangle.
     *  @param rotation The rotation of the rectangle.
     *  @param origin The origin point for the rotation. (relative to the position of the rectangle) */
    [[nodiscard]]
    inline bool PointInRect(const Vector2f& point, const Vector2f& pos, const Vector2f& size, float rotation, const Vector2f& origin) noexcept
    {
        const Vector2f rp = Rotate(pos + origin, point, -rotation);
        return (InRange(rp.x, pos.x, pos.x + size.x) && InRange(rp.y, pos.y, pos.y + size.y));
    }

    /** @brief Checks whether a point is within a circle.
     *  @param point The point.
     *  @param pos The center of the circle.
     *  @param radius The radius of the circle. */
    [[nodiscard]]
    inline bool PointInCircle(const Vector2f& point, const Vector2f& pos, float radius) noexcept
    {
        return (Distance(point, pos) <= radius);
    }

    /** @brief Normalizes a vector. */
    template<typename T>
    [[nodiscard]]
    inline constexpr Vector2<T> Normalize(const Vector2<T>& v) noexcept
    {
        return v.Normalized();
    }

    /** @brief Generates a random value within a given range.
     *
     *  @tparam T Arithmetic value type
     *  @param min Minimum value
     *  @param max Maximum value
     *  @return Random value between minimum and maximum */
    template<typename T = int32_t>
    [[nodiscard]]
    inline constexpr typename std::enable_if<std::is_arithmetic<T>::value, T>::type Random(T min = std::numeric_limits<T>::min(), T max = std::numeric_limits<T>::max()) noexcept
    {
        if constexpr(std::is_floating_point<T>::value)
        {
            std::uniform_real_distribution<T> distribution(min, max);

            return distribution(Impl::randomGenerator);
        }
        else if constexpr(std::is_integral<T>::value)
        {
            std::uniform_int_distribution<T> distribution(min, max);

            return distribution(Impl::randomGenerator);
        }
    }

    template<typename T, typename ValueType = typename T::value_type>
    inline ValueType AvgOf(const T& container)
    {
        ValueType value{};

        for(const auto& i : container)
        {
            value = value + i;
        }

        return value / container.size();
    }

    template<typename T>
    inline T AvgOf(const std::initializer_list<T>& initializerList)
    {
        T value{};

        for(const auto& i : initializerList)
        {
            value = value + i;
        }

        return value / initializerList.size();
    }

    template<typename ... Args>
    inline typename std::common_type<Args...>::type AvgOf(const Args&... args)
    {
        return AvgOf({args...});
    }

    template<typename T, typename ValueType = typename T::value_type>
    inline ValueType MaxOf(const T& container)
    {
        auto value = std::numeric_limits<ValueType>::min();

        for(auto& i : container)
        {
            value = i > value ? i : value;
        }
            
        return value;
    }

    template<typename T>
    inline T MaxOf(const std::initializer_list<T>& initializerList)
    {
        T value = std::numeric_limits<T>::min();

        for(const auto& i : initializerList)
        {
            value = i > value ? i : value;
        }

        return value;
    }

    template<typename ... Args>
    inline typename std::common_type<Args...>::type MaxOf(const Args&... args)
    {
        return MaxOf({args...});
    }

    template<typename T, typename ValueType = typename T::value_type>
    inline ValueType MinOf(const T& container)
    {
        auto value = std::numeric_limits<ValueType>::max();

        for(auto& i : container)
        {
            value = i < value ? i : value;
        }
            
        return value;
    }

    template<typename T>
    inline T MinOf(const std::initializer_list<T>& initializerList)
    {
        T value = std::numeric_limits<T>::max();

        for(const auto& i : initializerList)
        {
            value = i < value ? i : value;
        }

        return value;
    }

    template<typename ... Args>
    inline typename std::common_type<Args...>::type MinOf(const Args&... args)
    {
        return MinOf({args...});
    }
}