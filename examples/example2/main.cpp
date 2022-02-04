#include <TML/TML.h>

using namespace tml;

int main()
{
    RenderWindow window(800, 600, "Example2");
    Sprite sprite;
    sprite.LoadFromFile("res/Logo.png");

    while(!window.ShouldClose())
    {
        auto event = window.PollEvents();
        if(event.type == Event::Closed)
            window.Close();

        window.Clear();
        window.Draw(sprite);
        window.Display();
    }
    return 0;
}