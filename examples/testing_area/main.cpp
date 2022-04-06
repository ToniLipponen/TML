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


    int colorIndex = 0;
    Clock clock;
    Vector2f pos = 0, pos2 = {300,300};
    bool down = false;

    while(window.IsOpen())
    {
        Event event{};
        while(window.PollEvents(event))
        {
            if(event.type == Event::Closed)
                window.Close();

            layout.Update(event);
        }
        auto offset2 = Gamepad::GetAxis(tml::Gamepad::Gamepad0, tml::Gamepad::RightJoystick);
        auto offset = Gamepad::GetAxis(tml::Gamepad::Gamepad0, tml::Gamepad::LeftJoystick);
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
        if(Gamepad::IsButtonPressed(tml::Gamepad::Gamepad0, Gamepad::Button0))
        {
            std::cout << 0 << std::endl;
        }
        if(Gamepad::IsButtonPressed(tml::Gamepad::Gamepad0, Gamepad::Button1))
        {
            std::cout << 1 << std::endl;
        }
        if(Gamepad::IsButtonPressed(tml::Gamepad::Gamepad0, Gamepad::Button2))
        {
            std::cout << 2 << std::endl;
        }
        if(Gamepad::IsButtonPressed(tml::Gamepad::Gamepad0, Gamepad::Button3))
        {
            std::cout << 3 << std::endl;
        }
        if(Gamepad::IsButtonPressed(tml::Gamepad::Gamepad0, Gamepad::Button4))
        {
            std::cout << 4 << std::endl;
        }
        if(Gamepad::IsButtonPressed(tml::Gamepad::Gamepad0, Gamepad::Button5))
        {
            std::cout << 5 << std::endl;
        }
        if(Gamepad::IsButtonPressed(tml::Gamepad::Gamepad0, Gamepad::Button6))
        {
            std::cout << 6 << std::endl;
        }
        if(Gamepad::IsButtonPressed(tml::Gamepad::Gamepad0, Gamepad::Button7))
        {
            std::cout << 7 << std::endl;
        }
        if(Gamepad::IsButtonPressed(tml::Gamepad::Gamepad0, Gamepad::Button8))
        {
            std::cout << 8 << std::endl;
        }
        if(Gamepad::IsButtonPressed(tml::Gamepad::Gamepad0, Gamepad::Button9))
        {
            std::cout << 9 << std::endl;
        }
        if(Gamepad::IsButtonPressed(tml::Gamepad::Gamepad0, Gamepad::Button10))
        {
            std::cout << 10 << std::endl;
        }
        if(Gamepad::IsButtonPressed(tml::Gamepad::Gamepad0, Gamepad::Button11))
        {
            std::cout << 11 << std::endl;
        }
        if(Gamepad::IsButtonPressed(tml::Gamepad::Gamepad0, Gamepad::Button12))
        {
            std::cout << 12 << std::endl;
        }
        if(Gamepad::IsButtonPressed(tml::Gamepad::Gamepad0, Gamepad::Button13))
        {
            std::cout << 13 << std::endl;
        }
        if(Gamepad::IsButtonPressed(tml::Gamepad::Gamepad0, Gamepad::Button14))
        {
            std::cout << 14 << std::endl;
        }
        if(Gamepad::IsButtonPressed(tml::Gamepad::Gamepad0, Gamepad::Button15))
        {
            std::cout << 15 << std::endl;
        }

        window.Clear();
        layout.Draw(window);
        window.DrawCircle(pos, 100, Color::Red);
        window.DrawCircle(pos2, 100, colors[colorIndex]);
        window.Display();
    }
    return 0;
}
