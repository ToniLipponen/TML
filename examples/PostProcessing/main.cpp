#include <TML/Graphics.h>
#include "ComputeShader.h"

using namespace tml;

int main()
{
    RenderWindow window(800, 600, "PostProcessing");
    RenderTexture renderTexture;
    renderTexture.SetClearColor(Color::White);
    renderTexture.Create(window.GetWidth(), window.GetHeight());

    ComputeShader shader;
    shader.LoadFromString(shaderSource);

    while(!window.ShouldClose())
    {
        Event e{};
        while(window.PollEvents(e))
        {
            switch(e.type)
            {
                case tml::Event::Closed:
                    window.Close();
                    break;

                default:
                    break;
            }
        }

        renderTexture.SetActive();
        renderTexture.Clear();
        renderTexture.DrawCircle(Mouse::GetPosition(), 200, Color::Green);
        renderTexture.Display();
        renderTexture.GetTexture().Bind(0);

        shader.Bind();
        shader.Uniform1i("image", 0);
        shader.Dispatch(window.GetWidth()/32, window.GetHeight()/8);
        shader.Wait();

        window.Clear();
        window.DrawTexture(renderTexture.GetTexture(), {0,0}, window.GetSize());
        window.Display();
    }
}