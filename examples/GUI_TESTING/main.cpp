#include <TML/TML.h>
#include <TML/Interface/Interface.h>

using namespace tml;
using namespace Interface;

/**
 * This is just a testing ground for GUI components.
 */

int main()
{
    RenderWindow window(800, 600, "GUI TESTING", Window::Resizeable);

    WindowContainer guiRoot(window);
    auto* hLayout = new HorizontalLayout(0,0,800, 600);
    auto* vLayout1 = new VerticalLayout(0,0,0,0);
    auto* vLayout2 = new VerticalLayout(0,0,0,0);
    auto* listbox = new Listbox(0,0,300,300);
    auto* view = new Viewport(0,0,400,200);

    view->AddListener("Update", [&](BaseComponent* c, Event& e)
    {
        auto& surface = ((Viewport*)c)->GetSurface();
        surface.SetActive();
        surface.Clear();
        surface.DrawText("Viewport component", {0,0}, 40, Color::Green);
        surface.Display();
        window.SetViewport({0, 0},window.GetSize());
    });

    vLayout1->AddChild(new Label(0,0,20, "Label"));
    vLayout1->AddChild(new Button(0,0,200,30,"Button"));
    vLayout1->AddChild(new TextInput(0,0,200,30));
    vLayout1->AddChild(new NumericInput<int>(0,0,200,30));
    vLayout1->AddChild(new Label(0,0,20, "Vertical slider"));
    vLayout1->AddChild(new VSlider(0,0,200));
    vLayout1->AddChild(new Spacer);
    vLayout1->AddChild(view);

    vLayout2->AddChild(new Label(0,0,20, "Horizontal slider"));
    vLayout2->AddChild(new HSlider(0,0,200));
    vLayout2->AddChild(new Label(0,0,20, "Listbox"));
    vLayout2->AddChild(listbox);
    vLayout2->AddChild(new Label(0,0,20, "Checkbox"));
    vLayout2->AddChild(new Checkbox(0,0,20));

    hLayout->AddChild(vLayout1);
    hLayout->AddChild(vLayout2);
    guiRoot.AddChild(hLayout);

    for(int i = 0; i < 20; i++)
    {
        listbox->AddValue("Listitem" + std::to_string(i));
    }
    Clock clock;
    clock.Reset();

    while(!window.ShouldClose())
    {
        Event event = window.PollEvents();
        if(event.type == Event::Closed)
            window.Close();

        window.Clear();
            guiRoot.Update(event, window);
        window.Display();
    }
    return 0;
}
