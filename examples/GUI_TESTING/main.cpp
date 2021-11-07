#include <TML/TML.h>
#include <TML/Interface/Interface.h>
using namespace tml;
using namespace Interface;

int main()
{
    Window window(800, 600, "GUI TESTING", Window::Resizeable | Window::Antialias);
    Renderer::Init();
    Renderer::SetClearColor(WHITE);

    Listbox list(0, 30, 200, 300);
    for(int i = 0; i < 20; i++)
        list.AddValue("Item" + std::to_string(i));

    Clock clock, clock2;
    double dt;

    Text fpsText;
    fpsText.SetColor(BLACK);

    while(!window.ShouldClose())
    {
        if(Keyboard::IsKeyPressed(Keyboard::KEY_A))
            list.AddValue("Item" + std::to_string(list.GetElementsCount()));
        else if(Keyboard::IsKeyPressed(Keyboard::KEY_D))
            list.RemoveValue(list.GetElementsCount()-1);

        Renderer::Clear();
        {
            list.Update();
            Renderer::Draw(fpsText);
        }
        Renderer::EndBatch();
        window.Display();
        dt = clock.Reset();
        if(clock2.GetTime() > 0.5)
        {
            fpsText.SetString("FPS: " + std::to_string(ui32(1.0 / dt)));
            clock2.Reset();
        }
    }
    return 0;
}