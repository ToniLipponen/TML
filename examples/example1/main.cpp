#include <TML/TML.h>

using namespace tml;

int main()
{
    Window window(800, 600, "Example1");
    Renderer::Init();

    while(!window.ShouldClose())
    {
        Renderer::Clear();
            Renderer::DrawCircle(window.GetSize() / 2, 100, GREEN);
        Renderer::EndBatch();
        window.Display();
    }
    return 0;
}