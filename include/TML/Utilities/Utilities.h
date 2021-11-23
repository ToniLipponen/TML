#pragma once
#include <TML/Utilities/Condition.h>
#include <TML/Utilities/String.h>
#include <TML/Utilities/Math.h>

namespace tml
{
    namespace Util
    {
        inline bool PointInRect(const Vector2f& point, const Vector2f& p, const Vector2f& s, float rotation)
        {
            const auto npoint = Math::Rotate(p + (s*0.5), point, -rotation);
            return (p < npoint && npoint < p+s);
        }

        inline bool PointInCircle(const Vector2f& point, const Vector2f& p, float r)
        {
            return (Math::Distance(point, p) <= r);
        }
    }
}