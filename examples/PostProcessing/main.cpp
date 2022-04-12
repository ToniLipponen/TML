#include <TML/Graphics.h>
using namespace tml;

const char* shaderSource = R"END(
#version 450 core
layout (local_size_x = 32, local_size_y = 8, local_size_z = 1) in;
layout (rgba8, binding = 0) uniform image2D image;
float vignette(vec2 uv, float radius, float smoothness)
{
    float diff = radius - distance(uv, vec2(0.5, 0.5));
    return smoothstep(-smoothness, smoothness, diff);
}
void main(void)
{
    ivec2 uv = ivec2(gl_GlobalInvocationID.xy);
    vec2 size = imageSize(image);
    vec4 color = imageLoad(image, uv) * vignette(vec2(uv)/size, 0.3, 0.3);
    color.a = 1;
    imageStore(image, uv, color);
}
)END";

int main()
{
    RenderWindow window(800, 600, "PostProcessing");

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

        shader.Bind();
        shader.Uniform1i("image", 0);
        shader.Dispatch(window.GetWidth()/32, window.GetHeight()/8, 1);
        shader.Wait();

        window.Clear();
        window.DrawTexture(texture, {0,0}, window.GetSize());
        window.Display();
    }
}