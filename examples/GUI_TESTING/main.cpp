#include <TML/TML.h>
#include <TML/Interface/Interface.h>
using namespace tml;
using namespace Interface;

/*
 * This is just a testing ground for GUI components.
 */

int main()
{
    Window window(800, 600, "GUI TESTING", Window::Resizeable);
    Renderer::Init();
    Renderer::SetClearColor(WHITE);

    Listbox* list = new Listbox(0, 30, 400, 300);
    DropMenu* dropmenu = new DropMenu(0,0, 400, 30);
    Progressbar* progressbar = new Progressbar(0,0, 400, 30);
    VerticalLayout* vlayout = new VerticalLayout(0,0,400,200);

    vlayout->AddChild(new Label(0,0,30));
    vlayout->AddChild(new Button(0,0,400, 30, "Button"));
    vlayout->AddChild(new TextInput(0,0,400, 30));
    vlayout->AddChild(new NumericInput<int>(0,0,400,30));
    vlayout->AddChild(progressbar);
    vlayout->AddChild(new Slider<Horizontal>(0,0, 400, 30));
    vlayout->AddChild(dropmenu);

    HorizontalLayout hlayout(0,0, 800, 200);
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
        Renderer::Clear();
        {
            hlayout.Update();
        }
        Renderer::EndBatch();
        window.Display();
        runTime += clock.Reset();
        progressbar->SetValue(runTime / 100.0);
    }
    return 0;
}