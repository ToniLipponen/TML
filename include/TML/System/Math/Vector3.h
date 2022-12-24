#pragma once
#include <cmath>
#include <cstdint>
#include <cassert>
#include <type_traits>

namespace tml
{
    template<typename T>
    class Vector3
    {
    public:
        inline constexpr Vector3() noexcept;
        inline constexpr Vector3(T v) noexcept;
        inline constexpr Vector3(T x, T y, T z) noexcept;

        template<typename R> inline constexpr Vector3(R v) noexcept;
        template<typename R> inline constexpr Vector3(R x, R y, R z) noexcept;
        template<typename R> inline constexpr Vector3(const Vector3<R>& v) noexcept;

        inline constexpr T& operator[](unsigned int index) noexcept;
        inline constexpr T operator[](unsigned int index) const noexcept;

        template<typename R> inline constexpr Vector3 operator+(const Vector3<R>& rhs) const noexcept;
        template<typename R> inline constexpr Vector3 operator-(const Vector3<R>& rhs) const noexcept;
        template<typename R> inline constexpr Vector3 operator/(const Vector3<R>& rhs) const noexcept;
        template<typename R> inline constexpr Vector3 operator*(const Vector3<R>& rhs) const noexcept;

        template<typename R> inline constexpr Vector3 operator+(R rhs) const noexcept;
        template<typename R> inline constexpr Vector3 operator-(R rhs) const noexcept;
        template<typename R> inline constexpr Vector3 operator/(R rhs) const noexcept;
        template<typename R> inline constexpr Vector3 operator*(R rhs) const noexcept;

        inline constexpr Vector3& operator+=(const Vector3& rhs) noexcept;
        inline constexpr Vector3& operator-=(const Vector3& rhs) noexcept;
        inline constexpr Vector3& operator*=(const Vector3& rhs) noexcept;
        inline constexpr Vector3& operator/=(const Vector3& rhs) noexcept;

        template<typename R> inline constexpr Vector3& operator+=(R rhs) noexcept;
        template<typename R> inline constexpr Vector3& operator-=(R rhs) noexcept;
        template<typename R> inline constexpr Vector3& operator*=(R rhs) noexcept;
        template<typename R> inline constexpr Vector3& operator/=(R rhs) noexcept;


        inline constexpr static float Length(const Vector3<T>& v) noexcept { return v.Length(); }
        inline constexpr static Vector3 Normalize(const Vector3<T>& v) noexcept { return v.Normalized(); };

        inline constexpr float Length() const noexcept;
        inline constexpr Vector3& Normalize() noexcept;
        inline constexpr Vector3 Normalized() const noexcept;

        template<typename R> inline constexpr double Dot(const Vector3<R>& rhs) const noexcept;
        template<typename R> inline constexpr float Cross(const Vector3<R>& rhs) const noexcept;

        template<typename R> inline constexpr Vector3<T>& operator=(const Vector3<R>& rhs) noexcept;
        template<typename R> inline constexpr Vector3<T>& operator=(const R rhs) noexcept;

        inline constexpr bool operator==(const Vector3& rhs) const noexcept;
        inline constexpr bool operator!=(const Vector3& rhs) const noexcept;

    public:
        T x = 0, y = 0, z = 0;
    };

    using Vector3i = Vector3<int>;
    using Vector3f = Vector3<float>;
    using Vector3d = Vector3<double>;

    /*
     * Vector3 implementation
     */


    template<typename T>
    inline constexpr Vector3<T>::Vector3() noexcept
    {
        static_assert(std::is_arithmetic<T>::value, "T in tml::Vector3<T> must be a numeric type");
    }

    template<typename T>
    inline constexpr Vector3<T>::Vector3(T v) noexcept
    {
        static_assert(std::is_arithmetic<T>::value, "T in tml::Vector3<T> must be a numeric type");

        x = v;
        y = v;
        z = v;
    }

    template<typename T>
    inline constexpr Vector3<T>::Vector3(T x, T y, T z) noexcept
    {
        static_assert(std::is_arithmetic<T>::value, "T in tml::Vector3<T> must be a numeric type");

        this->x = x;
        this->y = y;
        this->z = z;
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector3<T>::Vector3(R v) noexcept
    {
        static_assert(std::is_arithmetic<R>::value, "R in tml::Vector3<T>(R v) must be a numeric type");
        static_assert(std::is_arithmetic<T>::value, "T in tml::Vector3<T> must be a numeric type");

        x = (static_cast<T>(v));
        y = (static_cast<T>(v));
        z = (static_cast<T>(v));
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector3<T>::Vector3(R x, R y, R z) noexcept
    : x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z))
    {
        static_assert(std::is_arithmetic<R>::value, "R in tml::Vector3<T>(R x, R y) must be a numeric type");
        static_assert(std::is_arithmetic<T>::value, "T in tml::Vector3<T> must be a numeric type");
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector3<T>::Vector3(const Vector3<R>& v) noexcept
    : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z))
    {

    }

    template<typename T>
    inline constexpr T& Vector3<T>::operator[](unsigned int index) noexcept
    {
        switch(index)
        {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default:
                assert("Invalid index into Vector3");
                return x;
        }
    }

    template<typename T>
    inline constexpr T Vector3<T>::operator[](unsigned int index) const noexcept
    {
        switch(index)
        {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default:
                assert("Invalid index into Vector3");
                return x;
        }
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector3<T> Vector3<T>::operator+(const Vector3<R> &rhs) const noexcept {
        return {x + static_cast<T>(rhs.x), y + static_cast<T>(rhs.y), z + static_cast<T>(rhs.z)};
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector3<T> Vector3<T>::operator-(const Vector3<R> &rhs) const noexcept {
        return {x - static_cast<T>(rhs.x), y - static_cast<T>(rhs.y), z - static_cast<T>(rhs.z)};
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector3<T> Vector3<T>::operator/(const Vector3<R> &rhs) const noexcept {
        return {x / static_cast<T>(rhs.x), y / static_cast<T>(rhs.y), z / static_cast<T>(rhs.z)};
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector3<T> Vector3<T>::operator*(const Vector3<R> &rhs) const noexcept {
        return {x * static_cast<T>(rhs.x), y * static_cast<T>(rhs.y), z * static_cast<T>(rhs.z)};
    }


    template<typename T>
    template<typename R>
    inline constexpr Vector3<T> Vector3<T>::operator+(const R rhs) const noexcept
    {
        return {x + static_cast<T>(rhs), y + static_cast<T>(rhs), z + static_cast<T>(rhs)};
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector3<T> Vector3<T>::operator-(const R rhs) const noexcept
    {
        return {x - static_cast<T>(rhs), y - static_cast<T>(rhs), z - static_cast<T>(rhs)};
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector3<T> Vector3<T>::operator/(const R rhs) const noexcept
    {
        return {x / static_cast<T>(rhs), y / static_cast<T>(rhs), z / static_cast<T>(rhs)};
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector3<T> Vector3<T>::operator*(const R rhs) const noexcept
    {
        return {x * static_cast<T>(rhs), y * static_cast<T>(rhs), z * static_cast<T>(rhs)};
    }

    template<typename T>
    inline constexpr Vector3<T>& Vector3<T>::operator+=(const Vector3<T> &rhs) noexcept {
        this->x += rhs.x;
        this->y += rhs.y;
        this->z += rhs.z;
        return *this;
    }

    template<typename T>
    inline constexpr Vector3<T>& Vector3<T>::operator-=(const Vector3<T> &rhs) noexcept {
        this->x -= rhs.x;
        this->y -= rhs.y;
        this->z -= rhs.z;
        return *this;
    }

    template<typename T>
    inline constexpr Vector3<T>& Vector3<T>::operator*=(const Vector3<T> &rhs) noexcept {
        this->x *= rhs.x;
        this->y *= rhs.y;
        this->z *= rhs.z;
        return *this;
    }

    template<typename T>
    inline constexpr Vector3<T>& Vector3<T>::operator/=(const Vector3<T> &rhs) noexcept {
        this->x /= rhs.x;
        this->y /= rhs.y;
        this->z /= rhs.z;
        return *this;
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector3<T>& Vector3<T>::operator+=(const R rhs) noexcept {
        this->x += rhs;
        this->y += rhs;
        this->z += rhs;
        return *this;
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector3<T>& Vector3<T>::operator-=(const R rhs) noexcept {
        this->x -= rhs;
        this->y -= rhs;
        this->z -= rhs;
        return *this;
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector3<T>& Vector3<T>::operator*=(const R rhs) noexcept {
        this->x *= rhs;
        this->y *= rhs;
        this->z *= rhs;
        return *this;
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector3<T>& Vector3<T>::operator/=(const R rhs) noexcept {
        this->x /= rhs;
        this->y /= rhs;
        this->z /= rhs;
        return *this;
    }

    template<typename T>
    inline constexpr float Vector3<T>::Length() const noexcept {
        return sqrt(x * x + y * y + z * z);
    }

    template<typename T>
    inline constexpr Vector3<T>& Vector3<T>::Normalize() noexcept {
        return (*this /= Length());
    }

    template<typename T>
    inline constexpr Vector3<T> Vector3<T>::Normalized() const noexcept {
        return *this / Length();
    }

    template<typename T>
    template<typename R>
    double constexpr Vector3<T>::Dot(const Vector3<R>& rhs) const noexcept {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    template<typename T>
    template<typename R>
    constexpr float Vector3<T>::Cross(const Vector3<R>& rhs) const noexcept {

        return {
            (y * rhs.z) - (z * rhs.y),
            (z * rhs.x) - (x * rhs.z),
            (x * rhs.y) - (y * rhs.x)};
    }

    template<typename T>
    template<typename R>
    inline constexpr Vector3<T>& Vector3<T>::operator=(const Vector3<R>& rhs) noexcept
    {
        x = static_cast<T>(rhs.x);
        y = static_cast<T>(rhs.y);
        z = static_cast<T>(rhs.z);
        return *this;
    }

    template<typename T>
    template<typename R>
    constexpr Vector3<T> &Vector3<T>::operator=(const R rhs) noexcept
    {
        *this = {static_cast<T>(rhs)};
        return *this;
    }

    template<typename T>
    constexpr bool Vector3<T>::operator==(const Vector3 &rhs) const noexcept
    {
        return (x == rhs.x && y == rhs.y && z == rhs.z);
    }

    template<typename T>
    constexpr bool Vector3<T>::operator!=(const Vector3 &rhs) const noexcept
    {
        return (x != rhs.x || y != rhs.y || z != rhs.z);
    }
}