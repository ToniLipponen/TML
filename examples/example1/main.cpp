#include <TML/TML.h>

using namespace tml;

int main()
{
    Window window(800, 600, "Example1");
    Renderer::Init();

    Clock clock;
    while(!window.ShouldClose())
    {
        Renderer::Clear();
            Renderer::DrawRect({0,0}, {100, 100}, RED);
        Renderer::EndBatch();
        window.Display();
    }
    return 0;
}