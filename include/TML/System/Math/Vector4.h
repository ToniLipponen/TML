#pragma once
#include <cmath>
#include <cstdint>
#include <cassert>

namespace tml
{
    template<typename T>
    class Vector4
    {
    public:
        inline constexpr Vector4() noexcept;
        inline constexpr Vector4(T v) noexcept;
        inline constexpr Vector4(T x, T y, T z) noexcept;
        inline constexpr Vector4(T x, T y, T z, T w) noexcept;

        template<typename R> inline constexpr Vector4(R v) noexcept;
        template<typename R> inline constexpr Vector4(R x, R y, R z) noexcept;
        template<typename R> inline constexpr Vector4(R x, R y, R z, R w) noexcept;
        template<typename R> inline constexpr Vector4(const Vector4<R>& v) noexcept;

        inline constexpr T& operator[](unsigned int index) noexcept;
        inline constexpr T operator[](unsigned int index) const noexcept;

        template<typename R> inline constexpr Vector4 operator+(const Vector4<R>& rhs) const noexcept;
        template<typename R> inline constexpr Vector4 operator-(const Vector4<R>& rhs) const noexcept;
        template<typename R> inline constexpr Vector4 operator/(const Vector4<R>& rhs) const noexcept;
        template<typename R> inline constexpr Vector4 operator*(const Vector4<R>& rhs) const noexcept;

        template<typename R> inline constexpr Vector4 operator+(R rhs) const noexcept;
        template<typename R> inline constexpr Vector4 operator-(R rhs) const noexcept;
        template<typename R> inline constexpr Vector4 operator/(R rhs) const noexcept;
        template<typename R> inline constexpr Vector4 operator*(R rhs) const noexcept;

        inline constexpr Vector4& operator+=(const Vector4& rhs) noexcept;
        inline constexpr Vector4& operator-=(const Vector4& rhs) noexcept;
        inline constexpr Vector4& operator*=(const Vector4& rhs) noexcept;
        inline constexpr Vector4& operator/=(const Vector4& rhs) noexcept;

        template<typename R> inline constexpr Vector4& operator+=(R rhs) noexcept;
        template<typename R> inline constexpr Vector4& operator-=(R rhs) noexcept;
        template<typename R> inline constexpr Vector4& operator*=(R rhs) noexcept;
        template<typename R> inline constexpr Vector4& operator/=(R rhs) noexcept;

        inline constexpr static float Length(const Vector4<T>& v) noexcept { return v.Length(); }
        inline constexpr static Vector4 Normalize(const Vector4<T>& v) noexcept { return v.Normalized(); };

        inline constexpr float Length() const noexcept;
        inline constexpr Vector4& Normalize() noexcept;
        inline constexpr Vector4 Normalized() const noexcept;

        template<typename R> inline constexpr double Dot(const Vector4<R>& rhs) const noexcept;
        template<typename R> inline constexpr float Cross(const Vector4<R>& rhs) const noexcept;

        template<typename R> inline constexpr Vector4<T>& operator=(const Vector4<R>& rhs) noexcept;
        template<typename R> inline constexpr Vector4<T>& operator=(const R rhs) noexcept;

        inline constexpr bool operator==(const Vector4& rhs) const noexcept;
        inline constexpr bool operator!=(const Vector4& rhs) const noexcept;

    public:
        T x = 0, y = 0, z = 0, w = 0;
    };

    using Vector4i = Vector4<int>;
    using Vector4f = Vector4<float>;
    using Vector4d = Vector4<double>;

    /*
     * Vector4 implementation
     */

    template<typename T>
    inline constexpr Vector4<T>::Vector4() noexcept
    {
        static_assert(std::is_arithmetic<T>::value, "T in tml::Vector4<T> must be a numeric type");
    }

    template<typename T>
    inline constexpr Vector4<T>::Vector4(T v) noexcept
    {
        static_assert(std::is_arithmetic<T>::value, "T in tml::Vector4<T> must be a numeric type");

        x = v;
        y = v;
        z = v;
    }

    template<typename T>
    inline constexpr Vector4<T>::Vector4(T x, T y, T z) noexcept
    {
        static_assert(std::is_arithmetic<T>::value, "T in tml::Vector4<T> must be a numeric type");

        this->x = x;
        this->y = y;
        this->z = z;
        this->w = 1;
    }

    template<typename T>
    inline constexpr Vector4<T>::Vector4(T x, T y, T z, T w) noexcept
    {
        static_assert(std::is_arithmetic<T>::value, "T in tml::Vector4<T> must be a numeric type");

        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector4<T>::Vector4(R v) noexcept
    {
        static_assert(std::is_arithmetic<R>::value, "R in tml::Vector4<T>(R v) must be a numeric type");
        static_assert(std::is_arithmetic<T>::value, "T in tml::Vector4<T> must be a numeric type");

        x = (static_cast<T>(v));
        y = (static_cast<T>(v));
        z = (static_cast<T>(v));
        w = (static_cast<T>(v));
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector4<T>::Vector4(R x, R y, R z) noexcept
    : x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z)), w(1)
    {
        static_assert(std::is_arithmetic<R>::value, "R in tml::Vector4<T>(R x, R y) must be a numeric type");
        static_assert(std::is_arithmetic<T>::value, "T in tml::Vector4<T> must be a numeric type");
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector4<T>::Vector4(const Vector4<R>& v) noexcept
    : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)), w(static_cast<T>(v.w))
    {

    }

    template<typename T>
    inline constexpr T& Vector4<T>::operator[](unsigned int index) noexcept
    {
        switch(index)
        {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
            default:
                assert("Invalid index into Vector4");
                return x;
        }
    }

    template<typename T>
    inline constexpr T Vector4<T>::operator[](unsigned int index) const noexcept
    {
        switch(index)
        {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
            default:
                assert("Invalid index into Vector4");
                return x;
        }
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector4<T> Vector4<T>::operator+(const Vector4<R> &rhs) const noexcept {
        return {
            x + static_cast<T>(rhs.x),
            y + static_cast<T>(rhs.y),
            z + static_cast<T>(rhs.z),
            w + static_cast<T>(rhs.w)
        };
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector4<T> Vector4<T>::operator-(const Vector4<R> &rhs) const noexcept {
        return {
            x - static_cast<T>(rhs.x),
            y - static_cast<T>(rhs.y),
            z - static_cast<T>(rhs.z),
            w - static_cast<T>(rhs.w)
        };
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector4<T> Vector4<T>::operator/(const Vector4<R> &rhs) const noexcept {
        return {
            x / static_cast<T>(rhs.x),
            y / static_cast<T>(rhs.y),
            z / static_cast<T>(rhs.z),
            w / static_cast<T>(rhs.w)
        };
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector4<T> Vector4<T>::operator*(const Vector4<R> &rhs) const noexcept {
        return {
            x * static_cast<T>(rhs.x),
            y * static_cast<T>(rhs.y),
            z * static_cast<T>(rhs.z),
            w * static_cast<T>(rhs.w)
        };
    }


    template<typename T>
    template<typename R>
    inline constexpr Vector4<T> Vector4<T>::operator+(const R rhs) const noexcept
    {
        return {
            x + static_cast<T>(rhs),
            y + static_cast<T>(rhs),
            z + static_cast<T>(rhs),
            w + static_cast<T>(rhs)
        };
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector4<T> Vector4<T>::operator-(const R rhs) const noexcept
    {
        return {
            x - static_cast<T>(rhs),
            y - static_cast<T>(rhs),
            z - static_cast<T>(rhs),
            w - static_cast<T>(rhs)
        };
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector4<T> Vector4<T>::operator/(const R rhs) const noexcept
    {
        return {
            x / static_cast<T>(rhs),
            y / static_cast<T>(rhs),
            z / static_cast<T>(rhs),
            w / static_cast<T>(rhs)
        };
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector4<T> Vector4<T>::operator*(const R rhs) const noexcept
    {
        return {
            x * static_cast<T>(rhs),
            y * static_cast<T>(rhs),
            z * static_cast<T>(rhs),
            w * static_cast<T>(rhs)
        };
    }

    template<typename T>
    inline constexpr Vector4<T>& Vector4<T>::operator+=(const Vector4<T> &rhs) noexcept {
        this->x += rhs.x;
        this->y += rhs.y;
        this->z += rhs.z;
        this->w += rhs.w;
        return *this;
    }

    template<typename T>
    inline constexpr Vector4<T>& Vector4<T>::operator-=(const Vector4<T> &rhs) noexcept {
        this->x -= rhs.x;
        this->y -= rhs.y;
        this->z -= rhs.z;
        this->w -= rhs.w;
        return *this;
    }

    template<typename T>
    inline constexpr Vector4<T>& Vector4<T>::operator*=(const Vector4<T> &rhs) noexcept {
        this->x *= rhs.x;
        this->y *= rhs.y;
        this->z *= rhs.z;
        this->w *= rhs.w;
        return *this;
    }

    template<typename T>
    inline constexpr Vector4<T>& Vector4<T>::operator/=(const Vector4<T> &rhs) noexcept {
        this->x /= rhs.x;
        this->y /= rhs.y;
        this->z /= rhs.z;
        this->w /= rhs.w;
        return *this;
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector4<T>& Vector4<T>::operator+=(const R rhs) noexcept {
        this->x += rhs;
        this->y += rhs;
        this->z += rhs;
        this->w += rhs;
        return *this;
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector4<T>& Vector4<T>::operator-=(const R rhs) noexcept {
        this->x -= rhs;
        this->y -= rhs;
        this->z -= rhs;
        this->w -= rhs;
        return *this;
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector4<T>& Vector4<T>::operator*=(const R rhs) noexcept {
        this->x *= rhs;
        this->y *= rhs;
        this->z *= rhs;
        this->w *= rhs;
        return *this;
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector4<T>& Vector4<T>::operator/=(const R rhs) noexcept {
        this->x /= rhs;
        this->y /= rhs;
        this->z /= rhs;
        this->w /= rhs;
        return *this;
    }

    template<typename T>
    inline constexpr float Vector4<T>::Length() const noexcept {
        return sqrt(x * x + y * y + z * z + w * w);
    }

    template<typename T>
    inline constexpr Vector4<T>& Vector4<T>::Normalize() noexcept {
        return (*this /= Length());
    }

    template<typename T>
    inline constexpr Vector4<T> Vector4<T>::Normalized() const noexcept {
        return *this / Length();
    }

    template<typename T>
    template<typename R>
    double constexpr Vector4<T>::Dot(const Vector4<R>& rhs) const noexcept {
        return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
    }

    template<typename T>
    template<typename R>
    constexpr float Vector4<T>::Cross(const Vector4<R>& rhs) const noexcept {

        return {
                (y * rhs.z) - (z * rhs.y),
                (z * rhs.x) - (x * rhs.z),
                (x * rhs.y) - (y * rhs.x),
                1};
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector4<T>& Vector4<T>::operator=(const Vector4<R>& rhs) noexcept
    {
        x = static_cast<T>(rhs.x);
        y = static_cast<T>(rhs.y);
        z = static_cast<T>(rhs.z);
        z = static_cast<T>(rhs.w);
        return *this;
    }

    template<typename T>
    template<typename R>
    constexpr Vector4<T> &Vector4<T>::operator=(const R rhs) noexcept
    {
        *this = {static_cast<T>(rhs)};
        return *this;
    }

    template<typename T>
    constexpr bool Vector4<T>::operator==(const Vector4 &rhs) const noexcept
    {
        return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w);
    }

    template<typename T>
    constexpr bool Vector4<T>::operator!=(const Vector4 &rhs) const noexcept
    {
        return (x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w);
    }
}