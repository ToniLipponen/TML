#include <TML/TML.h>
#include "TML/Interface.h"
using namespace tml;
using namespace Interface;

int main()
{
    RenderWindow window(800, 600, "Example 3");

    Music music("res/Kevin MacLeod - Study and Relax.mp3");
    music.Play();

    while(!window.ShouldClose())
    {
        auto event = window.PollEvents();
        if(event.type == Event::Closed)
            window.Close();

        /// Toggle music playback with space bar.
        if(event.type == Event::KeyPressed && event.key.code == Keyboard::KEY_SPACE)
            music.IsPlaying() ? music.Pause() : music.Play();

        window.Clear();
        window.DrawText("Playing: Kevin MacLeod - Study and Relax", {10,10}, 40, Color::Green);
        window.Display();
    }
    return 0;
}
