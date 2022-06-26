#include <TML/Graphics.h>
#include <TML/System/Clock.h>

using namespace tml;

struct Scene : public Drawable
{
    void SetRayPosition(const Vector2f& pos)
    {
        ray.position = pos;
    }

    void OnDraw(RenderTarget* renderTarget, Texture*) noexcept override
    {
        const auto window = dynamic_cast<RenderWindow*>(renderTarget);

        shape.ClearPoints();
        shape.AddPoint(ShapePoint{ray.position, Color(255,255,255,200)});

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

            shape.AddPoint(ShapePoint{ray.position + ray.direction * nearest, Color(255,255,255,200)});
            i += 0.1f;
        }
        window->Draw(shape);
    }

    std::vector<Rectangle> rects;
    std::vector<Circle> circles;
    Shape shape;
    Ray ray;
};

int main()
{
    RenderWindow window(800, 600, "Ray casting", WindowSettings::Resizeable | WindowSettings::Antialias);
    const Vector2f windowSize2 = window.GetSize() / 2;

    Scene scene;
    scene.rects.emplace_back(Rectangle(0, 0, window.GetWidth(), window.GetHeight()));
    scene.rects.emplace_back(Rectangle(100, 100, 100, 100));
    scene.circles.emplace_back(Circle(windowSize2, 200.f));

    Clock clock;
    double delta = 0;

    while(window.IsOpen())
    {
        Event event{};

        while(window.PollEvents(event))
        {
            switch(event.type)
            {
                case tml::EventType::Closed:
                    window.Close();
                    break;
                case tml::EventType::MouseMoved:
                    scene.SetRayPosition(Vector2f(event.pos.x, event.pos.y));
                    break;
                case tml::EventType::WindowResized:
                    scene.rects.at(0) = Rectangle(0,0, event.size.w, event.size.h);
                    break;
                default:break;
            }
        }

        scene.rects.at(1).Rotate(delta * 100.0);

        window.Clear();
        window.DrawCircle(windowSize2, 200.f, Color::Green);
        window.DrawRect({100, 100}, {100, 100}, Color::Red, 0, scene.rects.at(1).GetRotation(), {50,50});
        window.Draw(scene);
        window.Display();

        delta = clock.Reset();
    }
}