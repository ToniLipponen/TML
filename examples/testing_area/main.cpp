#include <TML/TML.h>
using namespace tml;
using namespace Interface;

int main()
{
    RenderWindow window(800, 600, "Window", Window::Resizeable);

    HorizontalLayout hlayout(10, 30, 800, 600);
    hlayout.AddListener("WindowResized", [](BaseComponent* c, Event& e)
    {
        c->SetSize({e.size.w, e.size.h});
    });

    VerticalLayout* vlayout1, *vlayout2;
    hlayout.AddChild(vlayout1 = new VerticalLayout(10,30, 300, 500));
    hlayout.AddChild(vlayout2 = new VerticalLayout(10,30, 300, 500));
    DropList* dropList;
    Viewport* viewport;

    vlayout1->AddChild(new TextInput(10,10,300));
    vlayout1->AddChild(new TextInput(10,10,300));
    vlayout1->AddChild(new Button("Button1", 10, 90,300, 30));
    vlayout1->AddChild(new Button("Button2", 10, 90,300, 30));
    vlayout1->AddChild(new Button("Button3", 10, 90,300, 30));
    vlayout1->AddChild(new Slider<tml::Interface::Horizontal>(10, 90,300, 15));
    vlayout1->AddChild(dropList = new DropList(0, 0, 300));
    vlayout1->AddChild(viewport = new Viewport(0,0,300,200));


    vlayout2->AddChild(new TextInput(10,10,300));
    vlayout2->AddChild(new TextInput(10,10,300));
    vlayout2->AddChild(new Button("Button1", 10, 90,300, 30));
    vlayout2->AddChild(new Button("Button2", 10, 90,300, 30));
    vlayout2->AddChild(new Button("Button3", 10, 90,300, 30));
    vlayout2->AddChild(new Slider<tml::Interface::Horizontal>(10, 90,300, 15));
//    vlayout2->AddChild(new Spacer);
    vlayout2->AddChild(new Label(0, 0, 20));
    vlayout2->AddChild(new Slider<tml::Interface::Vertical>(0, 0, 300));

    window.SetClearColor(Color::White);

    for(auto i = 0; i < 20; i++)
        dropList->AddValue("Value " + std::to_string(i));

    Clock clock;
    double dt;
    uint64_t samples = 0;
    double accumulatedDelta = 0;

    while(window.IsOpen())
    {
        dt = clock.Reset();
        Event event{};
        while(window.PollEvents(event))
        {
            if(event.type == Event::Closed)
                window.Close();

            hlayout.Update(event);
        }

        auto& surface = viewport->GetSurface();
        surface.Clear();
        surface.DrawCircle({100, 100}, 50, Color::Red);
        surface.Display();

        window.Clear();
        hlayout.Draw(window);
        window.DrawText("FPS: " + std::to_string(int(1.0 / dt)), {10, 10}, 20, Color::Black);
        window.DrawText("Avg: " + std::to_string(int(1.0 / (accumulatedDelta / samples))), {100, 10}, 20, Color::Black);
        window.Display();
        samples++;
        accumulatedDelta += dt;
    }
    return 0;
}