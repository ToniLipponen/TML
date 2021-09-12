#pragma once
#include <cmath>
#include <ostream>

namespace tml {
    class Vector2 {
    public:
        Vector2() = default;
        Vector2(float v) noexcept : x(v), y(v) {}
        Vector2(float x, float y) noexcept : x{x}, y{y} {}

        inline Vector2 operator+(const Vector2 &rhs) const noexcept {
            return {x + rhs.x, y + rhs.y};
        }

        inline Vector2 operator-(const Vector2 &rhs) const noexcept {
            return {x - rhs.x, y - rhs.y};
        }

        inline Vector2 operator/(const Vector2 &rhs) const noexcept {
            return {x / rhs.x, y / rhs.y};
        }

        inline Vector2 operator*(const Vector2 &rhs) const noexcept {
            return {x * rhs.x, y * rhs.y};
        }

        inline Vector2 operator/(const float rhs) const noexcept {
            return {x / rhs, y / rhs};
        }

        inline Vector2 operator*(const float rhs) const noexcept {
            return {x * rhs, y * rhs};
        }

        inline constexpr Vector2 &operator+=(const Vector2 &rhs) noexcept {
            this->x += rhs.x;
            this->y += rhs.y;
            return *this;
        }

        inline constexpr Vector2 &operator-=(const Vector2 &rhs) noexcept {
            this->x -= rhs.x;
            this->y -= rhs.y;
            return *this;
        }

        inline constexpr Vector2 &operator*=(const Vector2 &rhs) noexcept {
            this->x *= rhs.x;
            this->y *= rhs.y;
            return *this;
        }

        inline constexpr Vector2 &operator/=(const Vector2 &rhs) noexcept {
            this->x /= rhs.x;
            this->y /= rhs.y;
            return *this;
        }

        inline constexpr Vector2 &operator*=(const float rhs) noexcept {
            this->x *= rhs;
            this->y *= rhs;
            return *this;
        }

        inline constexpr Vector2 &operator/=(const float rhs) noexcept {
            this->x /= rhs;
            this->y /= rhs;
            return *this;
        }
        friend std::ostream& operator<<(std::ostream& s, const Vector2& v);

        inline float Length() const noexcept {
            return sqrtf(x * x + y * y);
        }

        inline Vector2 &Normalize() noexcept {
            return (*this /= Length());
        }

        inline Vector2 Normalized() const noexcept {
            return *this / Length();
        }

        inline static float Distance(const Vector2 &a, const Vector2 &b) noexcept {
            return sqrtf(powf(b.x - a.x, 2) + powf(b.y - a.y, 2));
        }

        float x, y;
    };

    inline std::ostream& operator<<(std::ostream &out, const Vector2& v)
    {
        out << v.x << std::string(" - ") << v.y;
        return out;
    }
}