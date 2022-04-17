#include <TML/TML.h>
using namespace tml;
using namespace Interface;

int main()
{
    RenderWindow window(800, 600, "Window", Window::Resizeable);

    VerticalLayout layout(10,30, 300, 500);
    DropList* dropList;

    layout.AddChild(new TextInput(10,10,400));
    layout.AddChild(new TextInput(10,10,400));
    layout.AddChild(new Button("Button1", 10, 90,300, 30));
    layout.AddChild(new Button("Button2", 10, 90,300, 30));
    layout.AddChild(new Button("Button3", 10, 90,300, 30));
    layout.AddChild(new Slider<tml::Interface::Horizontal>(10, 90,300, 15));
    layout.AddChild(dropList = new DropList(0, 0, 300));
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

            layout.Update(event);
        }

        window.Clear();
        layout.Draw(window);
        window.DrawText("FPS: " + std::to_string(int(1.0 / dt)), {10, 10}, 20, Color::Black);
        window.DrawText("Avg: " + std::to_string(int(1.0 / (accumulatedDelta / samples))), {100, 10}, 20, Color::Black);
        window.Display();
        samples++;
        accumulatedDelta += dt;
    }
    return 0;
}