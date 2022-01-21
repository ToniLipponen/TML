#include <TML/TML.h>
#include <TML/Interface/Interface.h>
using namespace tml;
using namespace Interface;

int main()
{
    Window window(800, 600, "Example 3");
    Renderer::Init();

    Music sound("res/Kevin MacLeod - Study and Relax.mp3");

    while(!window.ShouldClose())
    {
        auto event = window.PollEvents();
        if(event.type == Event::Closed)
            window.Close();
        if(event.type == Event::KeyReleased)
            sound.Play();

        Renderer::Clear();
        Renderer::DrawText("Playing: Kevin MacLeod - Study and Relax", {10,10}, 40, GREEN);
        Renderer::EndBatch();
        window.Display();
    }
    return 0;
}