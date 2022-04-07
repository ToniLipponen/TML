#include <TML/TML.h>
#include <TML/Window/Gamepad.h>

using namespace tml;
using namespace Interface;

Color colors[] = {
        Color::Red, Color::Green, Color::Blue,
        Color::Yellow, Color::Cyan, Color::Magenta,
        Color::Black, Color::White
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
    Vector2f pos = 0, pos2 = {300,300};

    while(window.IsOpen())
    {
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
            pos.x += offset.x;
        if(offset.y > 0.1 || offset.y < -0.1)
            pos.y += offset.y;

        if(offset2.x > 0.1 || offset2.x < -0.1)
            pos2.x += offset2.x;
        if(offset2.y > 0.1 || offset2.y < -0.1)
            pos2.y += offset2.y;

        if(Gamepad::IsButtonPressed(tml::Gamepad::Gamepad0, Gamepad::Button0))
        {
            colorIndex = (colorIndex + 1) % (sizeof(colors) / sizeof(Color));
        }

        window.Clear();
        layout.Draw(window);
        window.DrawCircle(pos, 100 + Gamepad::GetTriggerValue(tml::Gamepad::Gamepad0, tml::Gamepad::LeftTrigger) * 100, Math::Lerp<Color>(Color::Red, Color::Blue, Gamepad::GetTriggerValue(Gamepad::Gamepad0, Gamepad::RightTrigger)));
        window.DrawCircle(pos2, 100, colors[colorIndex]);
        window.Display();
    }
    return 0;
}
