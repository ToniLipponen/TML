#include <TML/TML.h>

using namespace tml;

int main()
{
    Window window(800, 600, "Example1");
    Renderer::Init();

//    Renderer::SetClearColor(RED);
    Clock clock;
    while(!window.ShouldClose())
    {
        Renderer::Clear();
            Renderer::DrawRect({0,0}, {100, 100}, RED);
            Renderer::DrawCircle(Mouse::GetPosition(), 100, 0xffffffff);
        Renderer::EndBatch();
        window.Display();
//        std::cout << 1.0 / clock.Reset() << std::endl;
    }
    return 0;
}