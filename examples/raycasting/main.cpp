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
        for(ui16 i = 0; i < 360; i++)
        {
            ray.direction = Math::AngleToHeading(i);
            float nearest = 10000;
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
    Window window(800, 600, "Raycasting", Window::VSync);
    Renderer::Init();
    const Vector2f windowSize2 = window.GetSize() / 2;

    Scene scene;
    scene.rects.emplace_back(Rectangle(0,0,window.GetWidth(), window.GetHeight()));
    scene.rects.emplace_back(Rectangle(100,100,100,100));
    scene.circles.emplace_back(Circle(windowSize2, 200.f));

    Clock clock;
    double delta = 0;
    Vector2f mousePos;

    while(!window.ShouldClose())
    {
        delta = clock.Reset();
        auto event = window.PollEvents();
        if(event.type == Event::Closed)
            window.Close();
        else if(event.type == Event::MouseMoved)
            mousePos = {event.mouseMove.x, event.mouseMove.y};
        scene.rects.at(1).Rotate(delta * 100.0);

        Renderer::Clear();
            Renderer::DrawCircle(windowSize2, 200.f, 0x007700ff);
            Renderer::DrawRect({100, 100}, {100, 100}, 0x770000ff,0, scene.rects.at(1).GetRotation());
            scene.Raycast(mousePos);
        Renderer::EndBatch();
        window.Display();
    }
}