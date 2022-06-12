#include <TML/TML.h>
#include <iostream>

using namespace tml;
using namespace Interface;

int main()
{
    RenderWindow window(800, 600, "Window", Window::Resizeable | Window::Antialias);

    VerticalLayout layout(10, 10, 300, 400);
    Button* closeButton;
    layout.AddChild(new Label("Widgets",30));
    layout.AddChild(new HSeparator);
    layout.AddChild(new HorizontalLayout({new Label("Checkbox:", 20, 100), new Checkbox(0,0, 20)}));
    layout.AddChild(new HorizontalLayout({new Label("Slider:", 20, 100), new HSlider(0,0,200)}));
    layout.AddChild(new HorizontalLayout({new Label("TextInput:", 20, 100), new TextInput(0,0,200,20)}));
    layout.AddChild(new TextInput(10,10,200,30));
    layout.AddChild(new HSeparator);


    layout.AddChild(closeButton = new Button("Close app"));

    closeButton->SetRoundness(5);

    closeButton->AddListener("Click", [&](BaseComponent*, Event&)
    {
        window.Close();
    });
    window.SetClearColor(Color::White);

    Clock clock;

    while(window.IsOpen())
    {
        Event event{};
        while(window.PollEvents(event))
        {
            if(event.type == EventType::Closed)
                window.Close();

            layout.Update(event);
        }

        window.Clear();
        window.Draw(layout);
        window.DrawText("Hello world", {12, 402}, 40, Color::Black);
        window.DrawText("Hello world", {10, 400}, 40, Color::Green);
        window.Display();
    }
    return 0;
}