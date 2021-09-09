#pragma once
#include "Copy.h"
#include "Condition.h"
#include "../Vector2.h"
#include "../Camera.h"

namespace tml {
    namespace Util {
        inline Vector2 Rotate(const Vector2 &origin, Vector2 p, float r) {
            Vector2 np;
            r = -1.f * (r * 0.01744444f);
            p -= origin;
            const float cos_r = cosf(r);
            const float sin_r = sinf(r);
            np.x = origin.x + p.x * cos_r - p.y * sin_r;
            np.y = origin.y + p.x * sin_r + p.y * cos_r;
            return np;
        }

        template<typename T>
        inline constexpr T Clamp(T value, T min, T max) noexcept {
            if (value > max)
                return max;
            if (value < min)
                return min;
            return value;
        }

        template<typename T>
        inline constexpr T Min(T value, T min) noexcept {
            if (value < min)
                return min;
            return value;
        }

        template<typename T>
        inline constexpr T Max(T value, T max) noexcept {
            if (value > max)
                return max;
            return value;
        }

        // Needs types with +, - and * operators
        // T operator+(const T& rhs)
        // T operator-(const T& rhs)
        // T operator*(float rhs)
        template<typename T>
        inline constexpr T Lerp(const T &a, const T &b, float m) noexcept {
            return a + ((b - a) * m);
        }
        // @brief Checks whether a value is withing given range
        template <typename T>
        inline constexpr bool InRange(const T& min, const T& value, const T& max) noexcept
        {
            if(value >= min && value <= max)
                return true;
            return false;
        }

        // TODO
        // Figure this out.
        inline Vector2 ScreenToWorld(const Vector2& p, const Vector2& view_dimensions, Camera& camera) noexcept
        {
            return p + camera.GetPosition()  - (view_dimensions / 2) * (1.0f - camera.GetZoom());
        }
    };
};