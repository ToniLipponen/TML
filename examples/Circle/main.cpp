#include <TML/Graphics.h>
using namespace tml;

int main()
{
    RenderWindow window(800, 600, "Circle");

    while(!window.ShouldClose())
    {
        Event event{};

        /// Polling events in a while loop.
        /// This is because multiple events might be pushed to the event queue each frame.
        while(window.PollEvents(event))
        {
            if(event.type == Event::Closed)
                window.Close();
        }

        window.Clear();
        window.DrawCircle(window.GetSize() / 2, 200, Color::Red);
        window.Display();
    }
    return 0;
}