#include <TML/TML.h>
#include <TML/Interface/Interface.h>
using namespace tml;
using namespace Interface;

int main()
{
    Window window(800, 600, "GUI TESTING", Window::Resizeable | Window::Antialias);
    Renderer::Init();

    Listbox list(100, 100, 200, 300);
    for(int i = 0; i < 50; i++)
        list.AddValue("Item" + std::to_string(i));

    while(!window.ShouldClose())
    {
        if(Keyboard::IsKeyPressed(Keyboard::KEY_A))
            list.AddValue("Item" + std::to_string(list.GetElementsCount()));
        if(Keyboard::IsKeyPressed(Keyboard::KEY_D))
            list.RemoveValue(list.GetElementsCount()-1);

        Renderer::Clear();
        list.Update();
        Renderer::EndBatch();
        window.Display();
    }
    return 0;
}