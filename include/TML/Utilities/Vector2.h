#pragma once
#include <cmath>

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

        inline Vector2 operator+(const Vector2 &rhs) const noexcept;

        inline Vector2 operator-(const Vector2 &rhs) const noexcept;

        inline Vector2 operator/(const Vector2 &rhs) const noexcept;

        inline Vector2 operator*(const Vector2 &rhs) const noexcept;

        inline Vector2 operator/(const T rhs) const noexcept;

        inline Vector2 operator*(const T rhs) const noexcept;

        inline constexpr Vector2 &operator+=(const Vector2 &rhs) noexcept;

        inline constexpr Vector2 &operator-=(const Vector2 &rhs) noexcept;

        inline constexpr Vector2 &operator*=(const Vector2 &rhs) noexcept;

        inline constexpr Vector2 &operator/=(const Vector2 &rhs) noexcept;

        inline constexpr Vector2 &operator*=(const T rhs) noexcept;

        inline constexpr Vector2 &operator/=(const T rhs) noexcept;


        inline float Length() const noexcept;

        inline Vector2 &Normalize() noexcept;

        inline Vector2 Normalized() const noexcept;

        inline bool operator<(const Vector2& rhs) const noexcept;

        inline bool operator>(const Vector2& rhs) const noexcept;

        template<typename R>
        inline Vector2<T>& operator=(const Vector2<R>& rhs) noexcept;

    public:
        T x = 0, y = 0;
    };


    using Vector2i = Vector2<int>;
    using Vector2f = Vector2<float>;
    using Vector2d = Vector2<double>;

    // Vector2 implementation

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
    inline Vector2<T> Vector2<T>::operator/(const T rhs) const noexcept {
        return {x / rhs, y / rhs};
    }

    template<typename T>
    inline Vector2<T> Vector2<T>::operator*(const T rhs) const noexcept {
        return {x * rhs, y * rhs};
    }

    template<typename T>
    inline constexpr Vector2<T>& Vector2<T>::operator+=(const Vector2 &rhs) noexcept {
        this->x += rhs.x;
        this->y += rhs.y;
        return *this;
    }

    template<typename T>
    inline constexpr Vector2<T>& Vector2<T>::operator-=(const Vector2 &rhs) noexcept {
        this->x -= rhs.x;
        this->y -= rhs.y;
        return *this;
    }

    template<typename T>
    inline constexpr Vector2<T>& Vector2<T>::operator*=(const Vector2 &rhs) noexcept {
        this->x *= rhs.x;
        this->y *= rhs.y;
        return *this;
    }

    template<typename T>
    inline constexpr Vector2<T>& Vector2<T>::operator/=(const Vector2 &rhs) noexcept {
        this->x /= rhs.x;
        this->y /= rhs.y;
        return *this;
    }

    template<typename T>
    inline constexpr Vector2<T>& Vector2<T>::operator*=(const T rhs) noexcept {
        this->x *= rhs;
        this->y *= rhs;
        return *this;
    }

    template<typename T>
    inline constexpr Vector2<T>& Vector2<T>::operator/=(const T rhs) noexcept {
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

//    inline static float Distance(const Vector2 &a, const Vector2 &b) noexcept {
//        return sqrtf(powf(b.x - a.x, 2) + powf(b.y - a.y, 2));
//    }

    template<typename T>
    inline bool Vector2<T>::operator<(const Vector2<T>& rhs) const noexcept
    {
        return (x < rhs.x && y < rhs.y);
    }

    template<typename T>
    inline bool Vector2<T>::operator>(const Vector2& rhs) const noexcept
    {
        return (x > rhs.x && y > rhs.y);
    }

    template<typename T>
    template<typename R>
    inline Vector2<T>& Vector2<T>::operator=(const Vector2<R>& rhs) noexcept
    {
        x = static_cast<T>(rhs.x);
        y = static_cast<T>(rhs.y);
        return *this;
    }
}
