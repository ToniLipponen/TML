#include <TML/Graphics/Ray.h>
#include <TML/System/Math.h>
#include <map>
#include <algorithm>

namespace tml
{
    Ray::Ray() noexcept
    : position(0), direction(0)
    {

    }

    Ray::Ray(const Vector2f &pos, const Vector2f &dir) noexcept
    : position(pos), direction(dir)
    {

    }

    Ray::Intersection Ray::IntersectsRect(const Vector2f& pos, const Vector2f& size, float rotation) const noexcept
    {
        std::vector<Vector2f> points(4);
        const Vector2f origin = pos + (size * 0.5);

        points[0] = Math::Rotate(origin, pos, rotation);
        points[1] = Math::Rotate(origin, pos+Vector2f(size.x, 0), rotation);
        points[2] = Math::Rotate(origin, pos+size, rotation);
        points[3] = Math::Rotate(origin, pos+Vector2f(0, size.y), rotation);

        return IntersectsShape(points);
    }

    // Todo: Add both intersecting points to rayIntersection.points
    Ray::Intersection Ray::IntersectsCircle(const Vector2f &pos, float r) const noexcept
    {
        Intersection rayIntersection;

        Vector2f L = pos - position;
        const double tca = L.Dot(direction);
//        if(tca < 0.0f)
//            return rayIntersection;

        const auto d2 = static_cast<float>(L.Dot(L) - tca * tca);

        if(d2 > r*r)
            return rayIntersection;

        float thc = std::sqrt((r*r) - d2);
        auto t0 = static_cast<float>(tca - thc);
        auto t1 = static_cast<float>(tca + thc);

        if(t0 > t1)
        {
            std::swap(t0, t1);
        }

        if(t0 < 0 )
        {
            t0 = t1;

            if(t0 < 0)
            {
                return rayIntersection;
            }
        }

        float t = t0;
        const Vector2f hitPosition = position + (direction * t);
        rayIntersection.intersects = true;
        rayIntersection.points.push_back({hitPosition, t});
        return rayIntersection;
    }

    Ray::Intersection Ray::IntersectsLine(const Vector2f& a, const Vector2f& b) const noexcept
    {
        Intersection rayIntersection;
        const float x1 = a.x;
        const float y1 = a.y;
        const float x2 = b.x;
        const float y2 = b.y;

        const float x3 = position.x;
        const float y3 = position.y;
        const float x4 = position.x + direction.x;
        const float y4 = position.y + direction.y;

        const float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 -x4);

        if(den == 0)
        {
            return rayIntersection;
        }

        const float t =  ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
        const float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;

        if(t >= 0.0f && t < 1.0f && u > 0.0f)
        {
            const Vector2f hitPosition = {x1 + t * (x2 - x1),y1 + t * (y2 - y1)};
            rayIntersection.intersects = true;
            rayIntersection.points.push_back({hitPosition, static_cast<float>(Math::Distance(position, hitPosition))});
        }

        return rayIntersection;
    }

    Ray::Intersection Ray::IntersectsShape(const std::vector<Vector2f>& points) const noexcept
    {
        Ray::Intersection intersection, rayIntersection;

        for(size_t i = 0; i < points.size(); i++)
        {
            intersection = IntersectsLine(points.at(i), points.at((i+1) % (points.size())));
            if(intersection == true)
            {
                rayIntersection.intersects = true;
                rayIntersection.points.push_back(intersection[0]);
            }
        }

        auto compare = [](const Intersection::Info& a, const Intersection::Info& b)
        {
            return a.distance < b.distance;
        };

        std::sort(rayIntersection.points.begin(), rayIntersection.points.end(), compare);
        return rayIntersection;
    }
}