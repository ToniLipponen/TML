#include <TML/TML.h>
#include <TML/Graphics/Ray.h>

using namespace tml;
struct Scene
{
    std::vector<Rectangle> rects;
    std::vector<Circle> circles;

    void Raycast(const Vector2f& pos)
    {
        Ray ray(pos, 0);
        for(ui32 i = 0; i < 360; i++)
        {
            ray.direction = Math::AngleToHeading(i);
            float nearest = MAXFLOAT;
            for(auto& r : rects)
            {
                const auto hitInfo = ray.IntersectsRect(r.GetPosition(), r.GetSize(), r.GetRotation());
                if(hitInfo.intersects && hitInfo.points.at(0).distance < nearest)
                {
                    nearest = hitInfo.points.at(0).distance;
                }
            }
            for(auto& c : circles)
            {
                const auto hitInfo = ray.IntersectsCircle(c.GetPosition(), c.GetRadius());
                if(hitInfo.intersects && hitInfo.points.at(0).distance < nearest)
                {
                    nearest = hitInfo.points.at(0).distance;
                }
            }
            Renderer::DrawLine(pos, pos + ray.direction * nearest, 1, 0xffffff66, false);
        }
    }
};

int main()
{
    Window window(1920, 1080, "Raycasting");
    Renderer::Init();
    Scene scene;
    scene.rects.push_back(Rectangle(0,0,window.GetWidth(), window.GetHeight()));
    scene.rects.push_back(Rectangle(100,100,100,100));
    scene.circles.push_back(Circle(window.GetSize() / 2, 200.f));

    while(!window.ShouldClose())
    {
        Renderer::Clear();
            scene.Raycast(Mouse::GetPosition());
        Renderer::EndBatch();
        window.Display();
    }
}