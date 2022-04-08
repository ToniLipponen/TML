#include <TML/TML.h>
#include <TML/Window/Gamepad.h>

using namespace tml;
using namespace Interface;

Color colors[] = {
        Color::Red,     Color::Green,   Color::Blue,
        Color::Yellow,  Color::Cyan,    Color::Magenta,
        Color::Black,   Color::White,   Color(255,150, 0), /* Orange */
};

int main()
{
    RenderWindow window(800, 600, "Window", Window::Resizeable);

    VerticalLayout layout(10,10, 300, 500);
    layout.AddChild(new TextInput(10,10,400));
    layout.AddChild(new TextInput(10,10,400));
    layout.AddChild(new Button("Button", 10, 90,300, 30));
    layout.AddChild(new Button("Button", 10, 90,300, 30));
    layout.AddChild(new Button("Button", 10, 90,300, 30));
    layout.AddChild(new Slider<tml::Interface::Horizontal>(10, 90,300, 15));
    window.SetClearColor(Color::White);


    size_t colorIndex = 0;
    Clock clock;
    double dt;
    Vector2f pos = 0, pos2 = {300,300};

    while(window.IsOpen())
    {
        dt = clock.Reset();
        Event event{};
        while(window.PollEvents(event))
        {
            if(event.type == Event::Closed)
                window.Close();

            layout.Update(event);
        }

        auto offset2 = Gamepad::GetAxisValue(tml::Gamepad::Gamepad0, tml::Gamepad::RightJoystick);
        auto offset = Gamepad::GetAxisValue(tml::Gamepad::Gamepad0, tml::Gamepad::LeftJoystick);
        if(offset.x > 0.1 || offset.x < -0.1)
            pos.x += offset.x * dt * 500;
        if(offset.y > 0.1 || offset.y < -0.1)
            pos.y += offset.y * dt * 500;

        if(offset2.x > 0.1 || offset2.x < -0.1)
            pos2.x += offset2.x * dt * 500;
        if(offset2.y > 0.1 || offset2.y < -0.1)
            pos2.y += offset2.y * dt * 500;

        if(Gamepad::IsButtonPressed(tml::Gamepad::Gamepad0, Gamepad::Button0))
        {
            colorIndex = (colorIndex + 1) % (sizeof(colors) / sizeof(Color));
        }

        window.Clear();
        layout.Draw(window);
        window.DrawCircle(pos, 100.5 + Gamepad::GetTriggerValue(tml::Gamepad::Gamepad0, tml::Gamepad::LeftTrigger) * 100, Color::Black);
        window.DrawCircle(pos, 100 + Gamepad::GetTriggerValue(tml::Gamepad::Gamepad0, tml::Gamepad::LeftTrigger) * 100, Math::Lerp<Color>(Color::Red, Color::Blue, Gamepad::GetTriggerValue(Gamepad::Gamepad0, Gamepad::RightTrigger)));
        window.DrawCircle(pos2, 100.5, Color::Black);
        window.DrawCircle(pos2, 100, colors[colorIndex]);
        window.DrawText("FPS:" + std::to_string(int(1.0 / dt)), {10, 10}, 50, Color::Black);
        window.Display();
    }
    return 0;
}
