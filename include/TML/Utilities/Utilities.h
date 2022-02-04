#pragma once
#include <TML/Utilities/Condition.h>
#include <TML/Utilities/String.h>
#include <TML/Utilities/Math.h>

namespace tml
{
    namespace Util
    {
        inline bool PointInRect(const Vector2f& point, const Vector2f& p, const Vector2f& s, float rotation) noexcept
        {
            const auto rp = Math::Rotate(p + (s * 0.5), point, -rotation);
            return (Math::InRange(rp.x, p.x, p.x + s.x) && Math::InRange(rp.y, p.y, p.y + s.y));
        }

        inline bool PointInCircle(const Vector2f& point, const Vector2f& p, float r) noexcept
        {
            return (Math::Distance(point, p) <= r);
        }
    }
}