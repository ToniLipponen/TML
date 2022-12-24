#pragma once
#include <cmath>
#include <cstdint>
#include <cassert>

namespace tml
{
    template<typename T>
    class Vector2
    {
    public:
        inline constexpr Vector2() noexcept;
        inline constexpr Vector2(T v) noexcept;
        inline constexpr Vector2(T x, T y) noexcept;

        template<typename R> inline constexpr Vector2(R v) noexcept;
        template<typename R> inline constexpr Vector2(R x, R y) noexcept;
        template<typename R> inline constexpr Vector2(const Vector2<R>& v) noexcept;

        inline constexpr T& operator[](unsigned int index) noexcept;
        inline constexpr T operator[](unsigned int index) const noexcept;

        template<typename R> inline constexpr Vector2 operator+(const Vector2<R>& rhs) const noexcept;
        template<typename R> inline constexpr Vector2 operator-(const Vector2<R>& rhs) const noexcept;
        template<typename R> inline constexpr Vector2 operator/(const Vector2<R>& rhs) const noexcept;
        template<typename R> inline constexpr Vector2 operator*(const Vector2<R>& rhs) const noexcept;

        template<typename R> inline constexpr Vector2 operator+(R rhs) const noexcept;
        template<typename R> inline constexpr Vector2 operator-(R rhs) const noexcept;
        template<typename R> inline constexpr Vector2 operator/(R rhs) const noexcept;
        template<typename R> inline constexpr Vector2 operator*(R rhs) const noexcept;

        inline constexpr Vector2& operator+=(const Vector2& rhs) noexcept;
        inline constexpr Vector2& operator-=(const Vector2& rhs) noexcept;
        inline constexpr Vector2& operator*=(const Vector2& rhs) noexcept;
        inline constexpr Vector2& operator/=(const Vector2& rhs) noexcept;

        template<typename R> inline constexpr Vector2& operator+=(R rhs) noexcept;
        template<typename R> inline constexpr Vector2& operator-=(R rhs) noexcept;
        template<typename R> inline constexpr Vector2& operator*=(R rhs) noexcept;
        template<typename R> inline constexpr Vector2& operator/=(R rhs) noexcept;

        inline constexpr static float Length(const Vector2<T>& v) noexcept { return v.Length(); }
        inline constexpr static Vector2 Normalize(const Vector2<T>& v) noexcept { return v.Normalized(); };
        inline constexpr float Length() const noexcept;
        inline constexpr Vector2& Normalize() noexcept;
        inline constexpr Vector2 Normalized() const noexcept;

        template<typename R> inline constexpr double Dot(const Vector2<R>& rhs) const noexcept;
        template<typename R> inline constexpr float Cross(const Vector2<R>& rhs) const noexcept;
        template<typename R> inline constexpr Vector2<T>& operator=(const Vector2<R>& rhs) noexcept;
        template<typename R> inline constexpr Vector2<T>& operator=(R rhs) noexcept;
        inline constexpr bool operator==(const Vector2& rhs) const noexcept;
        inline constexpr bool operator!=(const Vector2& rhs) const noexcept;

    public:
        T x = 0, y = 0;
    };

    using Vector2i = Vector2<int32_t>;
    using Vector2ui = Vector2<uint32_t>;
    using Vector2f = Vector2<float>;
    using Vector2d = Vector2<double>;

    /*
     * Vector2 implementation
     */


    template<typename T>
    inline constexpr Vector2<T>::Vector2() noexcept
    {
        static_assert(std::is_arithmetic<T>::value, "T in tml::Vector2<T> must be a numeric type");
    }

    template<typename T>
    inline constexpr Vector2<T>::Vector2(T v) noexcept
    {
        static_assert(std::is_arithmetic<T>::value, "T in tml::Vector2<T> must be a numeric type");

        x = v;
        y = v;
    }

    template<typename T>
    inline constexpr Vector2<T>::Vector2(T x, T y) noexcept
    {
        static_assert(std::is_arithmetic<T>::value, "T in tml::Vector2<T> must be a numeric type");

        this->x = x;
        this->y = y;
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector2<T>::Vector2(R v) noexcept
    {
        static_assert(std::is_arithmetic<R>::value, "R in tml::Vector2<T>(R v) must be a numeric type");
        static_assert(std::is_arithmetic<T>::value, "T in tml::Vector2<T> must be a numeric type");

        x = (static_cast<T>(v));
        y = (static_cast<T>(v));
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector2<T>::Vector2(R x, R y) noexcept
    : x(static_cast<T>(x)), y(static_cast<T>(y))
    {
        static_assert(std::is_arithmetic<R>::value, "R in tml::Vector2<T>(R x, R y) must be a numeric type");
        static_assert(std::is_arithmetic<T>::value, "T in tml::Vector2<T> must be a numeric type");

        this->x = (static_cast<T>(x));
        this->y = (static_cast<T>(y));
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector2<T>::Vector2(const Vector2<R>& v) noexcept
    : x(static_cast<T>(v.x)), y(static_cast<T>(v.y))
    {

    }

    template<typename T>
    inline constexpr T& Vector2<T>::operator[](unsigned int index) noexcept
    {
        switch(index)
        {
            case 0: return x;
            case 1: return y;
            default:
                assert("Invalid index into Vector2");
                return x;
        }
    }

    template<typename T>
    inline constexpr T Vector2<T>::operator[](unsigned int index) const noexcept
    {
        switch(index)
        {
            case 0: return x;
            case 1: return y;
            default:
                assert("Invalid index into Vector2");
                return x;
        }
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector2<T> Vector2<T>::operator+(const Vector2<R> &rhs) const noexcept {
        return {x + static_cast<T>(rhs.x), y + static_cast<T>(rhs.y)};
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector2<T> Vector2<T>::operator-(const Vector2<R> &rhs) const noexcept {
        return {x - static_cast<T>(rhs.x), y - static_cast<T>(rhs.y)};
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector2<T> Vector2<T>::operator/(const Vector2<R> &rhs) const noexcept {
        return {x / static_cast<T>(rhs.x), y / static_cast<T>(rhs.y)};
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector2<T> Vector2<T>::operator*(const Vector2<R> &rhs) const noexcept {
        return {x * static_cast<T>(rhs.x), y * static_cast<T>(rhs.y)};
    }


    template<typename T>
    template<typename R>
    inline constexpr Vector2<T> Vector2<T>::operator+(const R rhs) const noexcept
    {
        return {x + static_cast<T>(rhs), y + static_cast<T>(rhs)};
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector2<T> Vector2<T>::operator-(const R rhs) const noexcept
    {
        return {x - static_cast<T>(rhs), y - static_cast<T>(rhs)};
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector2<T> Vector2<T>::operator/(const R rhs) const noexcept
    {
        return {x / static_cast<T>(rhs), y / static_cast<T>(rhs)};
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector2<T> Vector2<T>::operator*(const R rhs) const noexcept
    {
        return {x * static_cast<T>(rhs), y * static_cast<T>(rhs)};
    }

    template<typename T>
    inline constexpr Vector2<T>& Vector2<T>::operator+=(const Vector2<T> &rhs) noexcept {
        this->x += rhs.x;
        this->y += rhs.y;
        return *this;
    }

    template<typename T>
    inline constexpr Vector2<T>& Vector2<T>::operator-=(const Vector2<T> &rhs) noexcept {
        this->x -= rhs.x;
        this->y -= rhs.y;
        return *this;
    }

    template<typename T>
    inline constexpr Vector2<T>& Vector2<T>::operator*=(const Vector2<T> &rhs) noexcept {
        this->x *= rhs.x;
        this->y *= rhs.y;
        return *this;
    }

    template<typename T>
    inline constexpr Vector2<T>& Vector2<T>::operator/=(const Vector2<T> &rhs) noexcept {
        this->x /= rhs.x;
        this->y /= rhs.y;
        return *this;
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector2<T>& Vector2<T>::operator+=(const R rhs) noexcept {
        this->x += rhs;
        this->y += rhs;
        return *this;
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector2<T>& Vector2<T>::operator-=(const R rhs) noexcept {
        this->x -= rhs;
        this->y -= rhs;
        return *this;
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector2<T>& Vector2<T>::operator*=(const R rhs) noexcept {
        this->x *= rhs;
        this->y *= rhs;
        return *this;
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector2<T>& Vector2<T>::operator/=(const R rhs) noexcept {
        this->x /= rhs;
        this->y /= rhs;
        return *this;
    }

    template<typename T>
    inline constexpr float Vector2<T>::Length() const noexcept {
        return sqrt(x * x + y * y);
    }

    template<typename T>
    inline constexpr Vector2<T>& Vector2<T>::Normalize() noexcept {
        return (*this /= Length());
    }

    template<typename T>
    inline constexpr Vector2<T> Vector2<T>::Normalized() const noexcept {
        return *this / Length();
    }

    template<typename T>
    template<typename R>
    double constexpr Vector2<T>::Dot(const Vector2<R>& rhs) const noexcept {
        return x*rhs.x + y*rhs.y;
    }

    template<typename T>
    template<typename R>
    constexpr float Vector2<T>::Cross(const Vector2<R>& rhs) const noexcept {
        return (x*rhs.y) - (y*rhs.y);
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector2<T>& Vector2<T>::operator=(const Vector2<R>& rhs) noexcept
    {
        x = static_cast<T>(rhs.x);
        y = static_cast<T>(rhs.y);
        return *this;
    }

    template<typename T>
    template<typename R>
    constexpr Vector2<T> &Vector2<T>::operator=(const R rhs) noexcept
    {
        *this = {static_cast<T>(rhs)};
        return *this;
    }

    template<typename T>
    constexpr bool Vector2<T>::operator==(const Vector2 &rhs) const noexcept
    {
        return (x == rhs.x && y == rhs.y);
    }

    template<typename T>
    constexpr bool Vector2<T>::operator!=(const Vector2 &rhs) const noexcept
    {
        return x != rhs.x || y != rhs.y;
    }
}
