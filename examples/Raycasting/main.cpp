#include <TML/Graphics.h>
#include <TML/System/Clock.h>

using namespace tml;
struct Scene
{
    std::vector<Rectangle> rects;
    std::vector<Circle> circles;

    void Raycast(const Vector2f& pos, RenderWindow& window)
    {
        Ray ray(pos, 0);
        for(ui32 i = 0; i < 360; i++)
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
            window.DrawLine(pos, pos + ray.direction * nearest, 1, int(0xffffff66), false);
        }
    }
};

int main()
{
    RenderWindow window(800, 600, "Raycasting", Window::VSync);
    const Vector2f windowSize2 = window.GetSize() / 2;

    Scene scene;
    scene.rects.emplace_back(Rectangle(0,0,window.GetWidth(), window.GetHeight()));
    scene.rects.emplace_back(Rectangle(100,100,100,100));
    scene.circles.emplace_back(Circle(windowSize2, 200.f));

    Clock clock;
    double delta = 0;
    Vector2f mousePos;

    while(window.IsOpen())
    {
        Event event{};

        while(window.PollEvents(event))
        {
            if(event.type == tml::Event::Closed)
                window.Close();
            else if(event.type == tml::Event::MouseMoved)
                mousePos = Vector2f(event.mouseMove.x, event.mouseMove.y);
        }

        scene.rects.at(1).Rotate(delta * 100.0);

        window.Clear();
            window.DrawCircle(windowSize2, 200.f, 0x007700ff);
            window.DrawRect({100, 100}, {100, 100}, 0x770000ff,0, scene.rects.at(1).GetRotation());
            scene.Raycast(mousePos, window);
        window.Display();
        delta = clock.Reset();
    }
}