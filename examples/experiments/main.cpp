#include <TML/TML.h>
#include <iostream>

using namespace tml;
using namespace Interface;

int main()
{
    RenderWindow window(800, 600, "Window", Window::Resizeable);

    VerticalLayout layout(10, 10, 300, 400);
    HSlider* slider;
    Button* closeButton;
    layout.AddChild(new TextInput(10,10,200,30));
    layout.AddChild(new HorizontalLayout({new Label("Slider: ",20), new Button("Button1"), new Button("Button2"), new Button("Another button")}));
    layout.AddChild(new HorizontalLayout({new Label("Slider: ",20), slider = new HSlider(0, 0, 100, 20, 0, 100)}));
    layout.AddChild(new HorizontalLayout({new Label("Input box: ",20), new TextInput(0,0,300,20)}));
    layout.AddChild(closeButton = new Button("Close app"));

    slider->AddListener("MouseMoved", [](BaseComponent* c, Event&)
    {
        if(c->GetState().MouseDown != -1)
        {
            std::cout << ((Slider<ComponentAxis::Horizontal>*)c)->GetValue() << "\n";
        }
    });

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