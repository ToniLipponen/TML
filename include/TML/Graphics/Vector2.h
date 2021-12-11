/*
 * Copyright (C) 2021 Toni Lipponen
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 */

#pragma once
#include <cmath> // For sqrt()

namespace tml
{
    template<typename T>
    class Vector2
    {
    public:
        Vector2() = default;
        constexpr inline Vector2(T v) noexcept;
        constexpr inline Vector2(T x, T y) noexcept;

        template<typename R>
        constexpr inline Vector2(R v) noexcept;

        template<typename R>
        constexpr inline Vector2(R x, R y) noexcept;

        template<typename R>
        constexpr inline Vector2(const Vector2<R>& v) noexcept;

        inline Vector2 operator+(const Vector2& rhs) const noexcept;
        inline Vector2 operator-(const Vector2& rhs) const noexcept;
        inline Vector2 operator/(const Vector2& rhs) const noexcept;
        inline Vector2 operator*(const Vector2& rhs) const noexcept;

        template<typename R>
        inline constexpr Vector2 operator+(const R rhs) const noexcept;

        template<typename R>
        inline constexpr Vector2 operator-(const R rhs) const noexcept;

        template<typename R>
        inline constexpr Vector2 operator/(const R rhs) const noexcept;

        template<typename R>
        inline constexpr Vector2 operator*(const R rhs) const noexcept;

        inline constexpr Vector2& operator+=(const Vector2& rhs) noexcept;
        inline constexpr Vector2& operator-=(const Vector2& rhs) noexcept;
        inline constexpr Vector2& operator*=(const Vector2& rhs) noexcept;
        inline constexpr Vector2& operator/=(const Vector2& rhs) noexcept;

        template<typename R>
        inline constexpr Vector2& operator+=(const R rhs) noexcept;

        template<typename R>
        inline constexpr Vector2& operator-=(const R rhs) noexcept;

        template<typename R>
        inline constexpr Vector2& operator*=(const R rhs) noexcept;

        template<typename R>
        inline constexpr Vector2& operator/=(const R rhs) noexcept;

        inline float Length() const noexcept;
        inline Vector2 &Normalize() noexcept;
        inline Vector2 Normalized() const noexcept;

        template<typename R>
        inline constexpr float Dot(const Vector2<R>& rhs) const noexcept;

        template<typename R>
        inline constexpr float Cross(const Vector2<R>& rhs) const noexcept;

        inline bool operator<(const Vector2& rhs) const noexcept;
        inline bool operator>(const Vector2& rhs) const noexcept;

        template<typename R>
        inline constexpr Vector2<T>& operator=(const Vector2<R>& rhs) noexcept;

        template<typename R>
        inline constexpr Vector2<T>& operator=(const R rhs) noexcept;

//        inline constexpr bool operator==(const Vector2& rhs) const noexcept;
//        inline constexpr bool operator!=(const Vector2& rhs) const noexcept;

    public:
        T x = 0, y = 0;
    };

    using Vector2i = Vector2<int>;
    using Vector2f = Vector2<float>;
    using Vector2d = Vector2<double>;

    /*
     * Vector2 implementation
     */

    template<typename T>
    constexpr inline Vector2<T>::Vector2(T v) noexcept
    : x(v), y(v)
    {

    }

    template<typename T>
    constexpr inline Vector2<T>::Vector2(T x, T y) noexcept
    : x(x), y(y)
    {

    }

    template<typename T>
    template<typename R>
    constexpr inline Vector2<T>::Vector2(R v) noexcept
    : x(static_cast<T>(v)), y(static_cast<T>(v))
    {

    }

    template<typename T>
    template<typename R>
    constexpr inline Vector2<T>::Vector2(R x, R y) noexcept
    : x(static_cast<T>(x)), y(static_cast<T>(y))
    {

    }

    template<typename T>
    template<typename R>
    constexpr inline Vector2<T>::Vector2(const Vector2<R>& v) noexcept
    : x(static_cast<T>(v.x)), y(static_cast<T>(v.y))
    {

    }

    template<typename T>
    inline Vector2<T> Vector2<T>::operator+(const Vector2 &rhs) const noexcept {
        return {x + rhs.x, y + rhs.y};
    }

    template<typename T>
    inline Vector2<T> Vector2<T>::operator-(const Vector2 &rhs) const noexcept {
        return {x - rhs.x, y - rhs.y};
    }

    template<typename T>
    inline Vector2<T> Vector2<T>::operator/(const Vector2 &rhs) const noexcept {
        return {x / rhs.x, y / rhs.y};
    }

    template<typename T>
    inline Vector2<T> Vector2<T>::operator*(const Vector2 &rhs) const noexcept {
        return {x * rhs.x, y * rhs.y};
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
    inline float Vector2<T>::Length() const noexcept {
        return sqrtf(x * x + y * y);
    }

    template<typename T>
    inline Vector2<T>& Vector2<T>::Normalize() noexcept {
        return (*this /= Length());
    }

    template<typename T>
    inline Vector2<T> Vector2<T>::Normalized() const noexcept {
        return *this / Length();
    }

    template<typename T>
    template<typename R>
    constexpr float Vector2<T>::Dot(const Vector2<R> &rhs) const noexcept {
        return x*rhs.x + y*rhs.y;
    }

    template<typename T>
    template<typename R>
    constexpr float Vector2<T>::Cross(const Vector2<R> &rhs) const noexcept {
        return (x*rhs.y) - (y*rhs.y);
    }

    template<typename T>
    inline bool Vector2<T>::operator<(const Vector2<T>& rhs) const noexcept
    {
        return (x < rhs.x && y < rhs.y);
    }

    template<typename T>
    inline bool Vector2<T>::operator>(const Vector2<T>& rhs) const noexcept
    {
        return (x > rhs.x && y > rhs.y);
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
        return {static_cast<T>(rhs)};
    }

//    template<typename T>
//    constexpr bool Vector2<T>::operator==(const Vector2 &rhs) const noexcept
//    {
//        return (x == rhs.x && y = rhs.y);
//    }
//
//    template<typename T>
//    constexpr bool Vector2<T>::operator!=(const Vector2 &rhs) const noexcept
//    {
//        return !(*this == rhs);
//    }
}
