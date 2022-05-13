#include <TML/Graphics.h>
using namespace tml;

const char* shaderSource = R"END(
#version 310 es
layout (local_size_x = 32, local_size_y = 8, local_size_z = 1) in;
layout (rgba8, binding = 0) readonly mediump uniform image2D imageIn;
layout (rgba8, binding = 1) writeonly mediump uniform image2D imageOut;

float vignette(vec2 uv, float radius, float smoothness)
{
    float diff = radius - distance(uv, vec2(0.5, 0.5));
    return smoothstep(-smoothness, smoothness, diff);
}

void main(void)
{
    ivec2 uv = ivec2(gl_GlobalInvocationID.xy);
    vec2 size = vec2(imageSize(imageIn));
    vec4 color = imageLoad(imageIn, uv) * vignette(vec2(uv)/size, 0.3, 0.3);
    color.a = 1.0;
    imageStore(imageOut, uv, color);
}
)END";

int main()
{
    RenderWindow window(800, 600, "Post-processing");

    Texture processedTexture;
    processedTexture.LoadFromMemory(window.GetWidth(), window.GetHeight(), 4, nullptr);

    RenderTexture renderTexture;
    renderTexture.SetClearColor(Color::White);
    renderTexture.Create(window.GetWidth(), window.GetHeight());

    ComputeShader shader;
    shader.LoadFromString(shaderSource);

    while(window.IsOpen())
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

        renderTexture.Clear();
        renderTexture.DrawCircle(Mouse::GetPosition(), 200, Color::Green);
        renderTexture.Display();

        const Texture& texture = renderTexture.GetTexture();
        texture.BindToImageSlot(0);
        processedTexture.BindToImageSlot(1);

        shader.Bind();
        shader.Uniform1i("imageIn", 0);
        shader.Uniform1i("imageOut", 1);
        shader.Dispatch(window.GetWidth()/32, window.GetHeight()/8, 1);
        shader.Wait();

        window.Clear();
        window.DrawTexture(processedTexture, {0,0}, window.GetSize());
        window.Display();
    }
}