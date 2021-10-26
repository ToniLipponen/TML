#include <TML/TML.h>

using namespace tml;

int main()
{
    Window window(800, 600, "Example2");
    Renderer::Init();
    Sprite sprite;
    sprite.LoadFromFile("res/Logo.png");

    while(!window.ShouldClose())
    {
        Renderer::Clear();
            Renderer::Draw(sprite);
        Renderer::EndBatch();
        window.Display();
    }
    return 0;
}