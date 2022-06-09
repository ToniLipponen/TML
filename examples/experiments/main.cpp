#include <TML/TML.h>

using namespace tml;
using namespace Interface;

int main()
{
    RenderWindow window(800, 600, "Window", Window::Resizeable);

    HorizontalLayout layout(10, 10, 400, 400);

    Label* fpsLabel;
    TextInput* textInput1, *textInput2;
//    VerticalLayout layout(10,10, 300, 500);
    layout.AddChild(fpsLabel = new Label(0, 0, 30, "                "));
    layout.AddChild(new Label(300,300,30));
    layout.AddChild(textInput1 = new TextInput(10,10,100,30));
    layout.AddChild(textInput2 = new TextInput(10,10,200,30));
//    layout.AddChild(new Button("Button", 10, 90,300, 30));
//    layout.AddChild(new Button("Button", 10, 90,300, 30));
//    layout.AddChild(new Button("Button", 10, 90,300, 30));
//    layout.AddChild(new Checkbox(0,0,20));
    window.SetClearColor(Color::White);

    textInput1->SetRoundness(0);
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
        window.Display();

        fpsLabel->SetValue("FPS:" + std::to_string(int(1.0 / clock.Reset())));
    }
    return 0;
}