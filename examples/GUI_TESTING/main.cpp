#include <TML/TML.h>
#include <TML/Interface/Interface.h>
#include <TML/IO/Event.h>

using namespace tml;
using namespace Interface;

/*
 * This is just a testing ground for GUI components.
 */

int main()
{
    RenderWindow window(800, 600, "GUI TESTING", Window::Resizeable);
    window.SetClearColor(Color::White);

    HorizontalLayout hlayout(0,0, 800, 600);
    Listbox* list = new Listbox(0, 30, 400, 300);
    DropMenu* dropmenu = new DropMenu(0,0, 400, 30);
    Progressbar* progressbar = new Progressbar(0,0, 400, 30);
    VerticalLayout* vlayout = new VerticalLayout(0,0,400,600);

    vlayout->AddChild(new Label(0,0,30));
    vlayout->AddChild(new Button(0,0,400, 30, "Button"));
    vlayout->AddChild(new TextInput(0,0,400, 30));
    vlayout->AddChild(new NumericInput<int>(0,0,400,30));
    vlayout->AddChild(progressbar);
    vlayout->AddChild(new Slider<Horizontal>(0,0, 400, 15));
    vlayout->AddChild(dropmenu);
    vlayout->AddChild(new Checkbox(0,0,20));

    hlayout.AddChild(list);
    hlayout.AddChild(vlayout);

    for(int i = 0; i < 20; i++)
    {
        list->AddValue("Item" + std::to_string(i));
        dropmenu->AddValue("Item" + std::to_string(i));
    }

    Clock clock;
    clock.Reset();
    double runTime = 0;

    while(!window.ShouldClose())
    {
        Event event = window.PollEvents();
        if(event.type == Event::Closed)
            window.Close();

        window.Clear();
            hlayout.Update(event, window);
        window.Display();
        runTime += clock.Reset();
        progressbar->SetValue(runTime / 100.0);
    }
    return 0;
}
