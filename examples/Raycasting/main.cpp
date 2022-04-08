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
        Shape shape;
        shape.AddPoint(pos, Color::White);
        const float max = sqrt(pow(window.GetWidth(), 2) + pow(window.GetHeight(), 2));

        for(float i = 0; i <= 360.1f;)
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

            shape.AddPoint(pos + ray.direction * nearest, Color::White * Math::Map<float>(nearest, 0, max, 1, 0));
            i += 0.1f;
        }
        window.Draw(shape);
    }
};

int main()
{
    RenderWindow window(800, 600, "Ray casting", Window::VSync | Window::Resizeable | Window::Antialias);
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
            switch(event.type)
            {
                case tml::Event::Closed:
                    window.Close();
                    break;
                case tml::Event::MouseMoved:
                    mousePos = Vector2f(event.mouseMove.x, event.mouseMove.y);
                    break;
                case tml::Event::WindowResized:
                    scene.rects.at(0) = Rectangle(0,0, event.size.x, event.size.y);
                    break;
                default:break;
            }
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