#include <TML/TML.h>

using namespace tml;
using namespace Interface;

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

    while(window.IsOpen())
    {
        Event event{};
        while(window.PollEvents(event))
        {
            if(event.type == Event::Closed)
                window.Close();

            layout.Update(event);
        }

        window.Clear();
        layout.Draw(window);
        window.Display();
    }
    return 0;
}
