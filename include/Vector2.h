#pragma once
#include <cmath>

class Vector2
{
public:
    Vector2() = default;
    inline constexpr Vector2(float x, float y) : x(x), y(y){}
    Vector2(float& x, float& y) : x(x), y(y){}
    
    inline constexpr Vector2 operator+(const Vector2& rhs) const noexcept
    {
        return Vector2(x+rhs.x, y+rhs.y);
    }
    inline constexpr Vector2 operator-(const Vector2& rhs) const noexcept
    {
        return Vector2(x-rhs.x, y-rhs.y);
    }
    inline constexpr Vector2 operator/(const Vector2& rhs) const noexcept
    {
        return Vector2(x/rhs.x, y/rhs.y);
    }
    inline constexpr Vector2 operator*(const Vector2& rhs) const noexcept
    {
        return Vector2(x*rhs.x, y*rhs.y);
    }
    inline constexpr Vector2 operator/(const float rhs) const noexcept
    {
        return Vector2(x/rhs, y/rhs);
    }
    inline constexpr Vector2 operator*(const float rhs) const noexcept
    {
        return Vector2(x*rhs, y*rhs);
    }
    inline constexpr Vector2& operator+=(const Vector2& rhs) noexcept
    {
        this->x += rhs.x;
        this->y += rhs.y;
        return *this;
    }
    inline constexpr Vector2& operator-=(const Vector2& rhs) noexcept
    {
        this->x -= rhs.x;
        this->y -= rhs.y;
        return *this;
    }
    inline constexpr Vector2& operator*=(const Vector2& rhs) noexcept
    {
        this->x *= rhs.x;
        this->y *= rhs.y;
        return *this;
    }
    inline constexpr Vector2& operator/=(const Vector2& rhs) noexcept
    {
        this->x /= rhs.x;
        this->y /= rhs.y;
        return *this;
    }
    inline constexpr Vector2& operator*=(const float rhs) noexcept
    {
        this->x *= rhs;
        this->y *= rhs;
        return *this;
    }
    inline constexpr Vector2& operator/=(const float rhs) noexcept
    {
        this->x /= rhs;
        this->y /= rhs;
        return *this;
    }
    inline constexpr float Length() const noexcept
    {
        return sqrt(x*x+y*y);
    }
    inline constexpr Vector2& Normalize() noexcept
    {
        return (*this /= Length());
    }
    inline constexpr Vector2 Normalized() const noexcept
    {
        return *this / Length();
    }
    inline constexpr static float Distance(const Vector2& a, const Vector2& b) noexcept
    {
        return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
    }

    float x, y;
};