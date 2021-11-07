#include <TML/TML.h>
#include <TML/Interface/Interface.h>
using namespace tml;
using namespace Interface;

int main()
{
    Window window(800, 600, "GUI TESTING", Window::Resizeable | Window::Antialias);
    Renderer::Init();
    Renderer::SetClearColor(WHITE);

    TextInput* input = new TextInput(0,0, 200);
    Listbox list(300, 300, 200, 300);
    for(int i = 0; i < 20; i++)
        list.AddValue("Item" + std::to_string(i));

    list.AddChild(input);
    Clock clock;
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
            Renderer::DrawRect(Mouse::GetPosition(), {200, 200}, RED, 10, 0);
            Renderer::Draw(fpsText);
        }
        Renderer::EndBatch();
        window.Display();
        dt = clock.Reset();
        fpsText.SetString("FPS: " + std::to_string(ui32(1.0 / dt)));
    }
    return 0;
}