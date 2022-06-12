#include <TML/TML.h>
#include <iostream>

using namespace tml;
using namespace Interface;

int main()
{
#if 1
    RenderWindow window(800, 600, "Window", Window::Resizeable | Window::Antialias);
#else
    RenderWindow window(800, 600, "Window", Window::Resizeable);
#endif

    VerticalLayout layout(10, 10, 300, 400);

    Button* closeButton;
    Combobox* list;
    ToggleSwitch* toggle;
    layout.AddChild(new Label("Widgets",30));
    layout.AddChild(new HSeparator);
    layout.AddChild(new HorizontalLayout({new Checkbox(0,0, 20), new Label("Option 1", 20, 100)}));
    layout.AddChild(new HorizontalLayout({new Checkbox(0,0, 20), new Label("Option 2", 20, 100)}));
    layout.AddChild(new HorizontalLayout({new Checkbox(0,0, 20), new Label("Option 3", 20, 100)}));
    layout.AddChild(new HorizontalLayout({new Checkbox(0,0, 20), new Label("Option 4", 20, 100)}));
    layout.AddChild(new HSeparator);

    layout.AddChild(new HorizontalLayout({new Label("Checkbox:", 20, 100), new Checkbox(0,0, 20)}));
    layout.AddChild(new HorizontalLayout({new Label("Slider:", 20, 100), new HSlider(0,0,200)}));
    layout.AddChild(new HorizontalLayout({new Label("TextInput:", 20, 100), new TextInput(0,0,200,20)}));
    layout.AddChild(new HorizontalLayout({new Label("TextInput:", 20, 100), toggle = new ToggleSwitch}));
    layout.AddChild(new TextInput(10,10,200,30));
    layout.AddChild(list = new Combobox(10, 10, 200, 20));
    layout.AddChild(new HSeparator);
//    toggle->SetRoundness(0);

    for(int i = 0; i < 10; i++)
        list->AddValue("ListItem " + std::to_string(i));

    layout.AddChild(closeButton = new Button("Close app"));

    closeButton->SetRoundness(100);

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