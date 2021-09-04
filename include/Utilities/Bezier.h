#pragma once
#include "Utilities.h"

namespace tml {
    template<typename T>
    inline constexpr T Quadratic(const T &p0, const T &p1, const T &p2, const float t) {
        return Util::Lerp(
                Util::Lerp(p0, p1, t),
                Util::Lerp(p1, p2, t),
                t);
    }

    template<typename T>
    inline constexpr T Cubic(const T &p0, const T &p1, const T &p2, const T &p3, const float t) {
        return Util::Lerp(
                Quadratic(p0, p1, p2, t),
                Quadratic(p1, p2, p3, t),
                t);
    }
};