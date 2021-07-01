#pragma once
// Header including some useful utilities

#include "Copy.h"
#include "Condition.h"
#include "../Vector2.h"


inline Vector2 Rotate(const Vector2& origin, Vector2 p, float r)
{
    Vector2 np;
    r = -1.f * (r*0.01744444f);
    p -= origin;
    float cos_r = cos(r);
    float sin_r = sin(r);
    np.x = origin.x + p.x * cos_r - p.y * sin_r;
    np.y = origin.y + p.x * sin_r + p.y * cos_r;
    return np;
}