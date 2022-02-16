#include <TML/TML.h>

using namespace tml;

int main()
{
    RenderWindow window(800, 600, "Sprite");
    Sprite sprite;
    sprite.LoadFromFile("Logo.png");

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