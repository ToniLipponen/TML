#include <TML/TML.h>
#include <iostream>

using namespace tml;
using namespace Interface;

int main()
{
    RenderWindow window(320, 420, "Window", Window::VSync);

    VerticalLayout layout(10, 10, 300, 450);

    Combobox* combobox;
    Listbox* listbox;
    layout.AddChild(new Label("Widgets",25));
    layout.AddChild(new HSeparator);
    layout.AddChild(new HorizontalLayout({new Checkbox(0,0, 20), new Label("Option 1")}));
    layout.AddChild(new HorizontalLayout({new Checkbox(0,0, 20), new Label("Option 2")}));
    layout.AddChild(new HorizontalLayout({new Checkbox(0,0, 20), new Label("Option 3")}));
    layout.AddChild(new HorizontalLayout({new Checkbox(0,0, 20), new Label("Option 4")}));
    layout.AddChild(new HSeparator);

    layout.AddChild(new HorizontalLayout({new Label("Checkbox:",    20, 125), new Checkbox(0,0, 20)}));
    layout.AddChild(new HorizontalLayout({new Label("Slider:",      20, 125), new HSlider(0,0,200)}));
    layout.AddChild(new HorizontalLayout({new Label("TextInput:",   20, 125), new TextInput(0,0,200,20)}));
    layout.AddChild(new HorizontalLayout({new Label("ToggleSwitch:",20, 125), new ToggleSwitch}));
    layout.AddChild(new HorizontalLayout({new Label("Button:",      20, 125), new Button("Click me!")}));
    layout.AddChild(new HorizontalLayout({new Label("Combobox:",    20, 125), combobox = new Combobox(10, 10,160)}));

    layout.AddChild(new HSeparator);
    layout.AddChild(new Label("Listbox", 20));
    layout.AddChild(listbox = new Listbox(0,0,300,300));

    for(int i = 0; i < 20; i++)
    {
        listbox->AddValue("ListItem " + std::to_string(i));
        combobox->AddValue("ListItem " + std::to_string(i));
    }

    while(window.IsOpen())
    {
        Event event{};

        while(window.PollEvents(event))
        {
            if(event.type == EventType::Closed)
            {
                window.Close();
            }

            layout.Update(event);
        }

        window.Clear();
        window.Draw(layout);
        window.Display();
    }
    return 0;
}