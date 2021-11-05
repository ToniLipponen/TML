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
    auto* pb = new Progressbar(100, 100, 500);
    VerticalLayout layout(10, 100, 300, 500);
    layout.AddChild(pb);

    while(!window.ShouldClose())
    {
        Renderer::Clear();
        layout.Update();
        pb->SetValue(music.GetProgress());
        Renderer::DrawText("Playing: Kevin MacLeod - Study and Relax", {10,10}, 40, GREEN);
        Renderer::EndBatch();
        window.Display();
    }
    std::cout << "Ended\n";
    return 0;
}