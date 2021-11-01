#include <TML/TML.h>

using namespace tml;

int main()
{
    Window window(800, 600, "Example 3");
    Renderer::Init();
    Music music("res/Kevin MacLeod - Study and Relax.ogg");
    music.Play();
    while(music.IsPlaying())
    {
        Renderer::Clear();
        Renderer::DrawText("Playing: Kevin MacLeod - Study and Relax", {10,10}, 40, GREEN);
        Renderer::EndBatch();
        window.Display();
    }
    return 0;
}