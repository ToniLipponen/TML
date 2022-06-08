#include <TML/Graphics.h>
#include <TML/Audio.h>
using namespace tml;

int main()
{
    RenderWindow window(800, 600, "Music");

    Music music("Kevin MacLeod - Study and Relax.mp3");
    music.Play();

    while(window.IsOpen())
    {
        Event event{};

        while(window.PollEvents(event))
        {
            if(event.type == EventType::Closed)
                window.Close();

            /// Toggle music playback with space bar.
            if(event.type == EventType::KeyPressed && event.key.value == Keyboard::Key::Space)
                music.IsPlaying() ? music.Pause() : music.Play();
        }

        window.Clear();
        window.DrawText("Playing: Kevin MacLeod - Study and Relax", {10,10}, 40, Color::Green);
        window.Display();
    }
    return 0;
}
