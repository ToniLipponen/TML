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
        auto event = window.PollEvents();
        if(event.type == Event::Closed)
            window.Close();

        Renderer::Clear();
            Renderer::Draw(sprite);
        Renderer::EndBatch();
        window.Display();
    }
    return 0;
}