#include <TML/TML.h>

using namespace tml;

int main()
{
    Window window(800, 600, "Example1");
    Renderer::Init();

    while(!window.ShouldClose())
    {
        auto event = window.PollEvents();
        if(event.type == Event::Closed)
            window.Close();

        Renderer::Clear();
            Renderer::DrawCircle(Mouse::GetPosition(), 100, GREEN);
        Renderer::EndBatch();
        window.Display();
    }
    return 0;
}