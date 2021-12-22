#include <TML/TML.h>
#include <TML/Interface/Interface.h>
using namespace tml;
using namespace Interface;

int main()
{
    Window window(800, 600, "Example 3");
    Renderer::Init();

    Music music("res/Kevin MacLeod - Study and Relax.mp3");
    music.Play();

    while(!window.ShouldClose())
    {
        auto event = window.PollEvents();
        if(event.type == Event::Closed)
            window.Close();

        Renderer::Clear();
        Renderer::DrawText("Playing: Kevin MacLeod - Study and Relax", {10,10}, 40, GREEN);
        Renderer::EndBatch();
        window.Display();
    }
    return 0;
}