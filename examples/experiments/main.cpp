#include <TML/Interface.h>
#include <iostream>

using namespace tml;
using namespace Interface;

int main()
{
    RenderWindow window(320, 500, "Window", Window::Resizeable);

    auto* layout = new VerticalLayout(0, 0, window.GetWidth(), window.GetHeight());

    layout->AddListener("WindowResized", [](BaseComponent* pointerToComponent, Event& resizeEvent)
    {
        auto* pointerToLayout = dynamic_cast<VerticalLayout*>(pointerToComponent);
        pointerToLayout->SetSize(resizeEvent.size.w, resizeEvent.size.h);
    });

    Combobox* combobox;
    Listbox* listbox;
    Menubar* menubar;

    layout->AddChild(menubar = new Menubar(window));
    layout->AddChild(new HorizontalLayout({new Label("Checkbox:",    20, 125), new Checkbox}));
    layout->AddChild(new HorizontalLayout({new Label("Slider:",      20, 125), new HSlider(100)}));
    layout->AddChild(new HorizontalLayout({new Label("TextInput:",   20, 125), new TextInput(200)}));
    layout->AddChild(new HorizontalLayout({new Label("ToggleSwitch:",20, 125), new ToggleSwitch}));
    layout->AddChild(new HorizontalLayout({new Label("Button:",      20, 125), new Button("Click me!")}));
    layout->AddChild(new HorizontalLayout({new Label("Combobox:",    20, 125), combobox = new Combobox(160)}));

    layout->AddChild(new HSeparator);
    layout->AddChild(new Label("Listbox", 20));
    layout->AddChild(listbox = new Listbox(300,300));


    for(int i = 0; i < 20; i++)
    {
        listbox->AddValue("ListItem" + std::to_string(i));
        combobox->AddValue("ListItem" + std::to_string(i));
    }

    for(int i = 0; i < 5; i++)
    {
        Button* button;
        menubar->AddChild(button = new Button("Item" + std::to_string(i)));
        button->SetRoundness(0);
    }

    UIRoot uiRoot;
    uiRoot.Attach(layout);

    while(window.IsOpen())
    {
        Event event{};

        while(window.PollEvents(event))
        {
            if(event.type == EventType::Closed)
            {
                window.Close();
            }

            uiRoot.Update(event);
        }

        window.Clear();
        window.Draw(uiRoot);
        window.Display();
    }

    return 0;
}

