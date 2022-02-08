#include <TML/TML.h>
using namespace tml;

int main()
{
    RenderWindow window(800, 600, "Example1");
    Text text;
    text.SetString("åöä");
    while(!window.ShouldClose())
    {
        auto event = window.PollEvents();
        if(event.type == Event::Closed)
            window.Close();

        window.Clear();
        window.DrawCircle(window.GetSize() / 2, 100, Color::Red);
        window.Draw(text);
        window.Display();
    }
    return 0;
}