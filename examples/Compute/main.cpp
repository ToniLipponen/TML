#include <TML/Graphics.h>
#include <TML/System.h>
using namespace tml;

struct PhysicsCircle
{
    Vector2f pos, velocity;
    double radius{};
};

const char* shaderSrc = R"END(
#version 430 core

layout( local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

struct Ball
{
    vec2 pos;
    vec2 momentum;
    float radius;
};

layout(std430, binding=0) buffer myBuffer{ Ball balls[]; };

uniform float delta;
uniform vec2 screenSize;
uniform vec2 mousePos;
uniform bool mouseDown;

void main (void)
{
    uint i = gl_GlobalInvocationID.x;

    float r = balls[i].radius;
    float dist = distance(balls[i].pos, mousePos);
    vec2 momentum = balls[i].momentum;

    if(mouseDown)
        balls[i].momentum -= ((balls[i].pos - mousePos) / (dist*dist)) * 50;
    else
        balls[i].momentum += ((balls[i].pos - mousePos) / (dist*dist)) * 50;

    if(balls[i].pos.x - r < 0)
    {
        balls[i].momentum.x = -momentum.x * 0.9;
        balls[i].pos.x = r;
    }
    else if(balls[i].pos.x + r > screenSize.x)
    {
        balls[i].momentum.x = -momentum.x * 0.9;
        balls[i].pos.x = screenSize.x - r;
    }
    if(balls[i].pos.y - r < 0)
    {
        balls[i].momentum.y = -momentum.y * 0.9;
        balls[i].pos.y = r;
    }
    else if(balls[i].pos.y + r > screenSize.y)
    {
        balls[i].momentum.y = -momentum.y * 0.9;
        balls[i].pos.y = screenSize.y - r;
    }
    balls[i].momentum *= 0.999;
    balls[i].pos += balls[i].momentum * delta;
}
)END";

int main()
{
    RenderWindow window(800, 600, "Compute", Window::Resizeable);

    std::vector<Color> circleColors;
    std::vector<PhysicsCircle> circles;

    constexpr static uint32_t CIRCLE_COUNT = 5000;

    for(auto i = 0; i < CIRCLE_COUNT; i++)
    {
        PhysicsCircle pc;
        pc.pos = Vector2f(rand() % window.GetWidth(), rand() % window.GetHeight());
        pc.velocity = Vector2f((rand() % 500) - 250);
        pc.radius = (rand() % 20);

        circles.push_back(pc);
        circleColors.emplace_back(rand() % 255, rand() % 255, rand() % 255);
        circleColors.at(circleColors.size()-1).a = 255;
    }

    StorageBuffer shaderData;
    shaderData.BufferData(circles.data(), circles.size() * sizeof(PhysicsCircle));

    ComputeShader shader;
    shader.LoadFromString(shaderSrc);

    Clock clock;
    Vector2f mousePos;

    while(!window.ShouldClose())
    {
        const auto delta = clock.Reset();
        Event event{};

        while(window.PollEvents(event))
        {
            if(event.type == tml::Event::Closed)
                window.Close();
            else if(event.type == tml::Event::MouseMoved)
                mousePos = Vector2f(event.mouseMove.x, event.mouseMove.y);
        }

        /// Bind the compute shader.
        shader.Bind();

        /// Update storage buffer & connect it to the compute shader.
        shaderData.UpdateData(circles.data(), circles.size() * sizeof(PhysicsCircle));
        shader.ConnectBuffer("myBuffer", 1, shaderData);

        /// Set uniforms.
        shader.Uniform1f("delta", delta);
        shader.Uniform2f("screenSize", window.GetWidth(), window.GetHeight());
        shader.Uniform2f("mousePos", mousePos.x, mousePos.y);
        shader.Uniform1i("mouseDown", Mouse::ButtonDown(Mouse::Left));

        /// Execute.
        shader.Dispatch(circles.size(), 1, 1);
        shader.Wait();

        /// Load data from the storage buffer back to circles data buffer.
        shaderData.RetrieveData(&circles[0], circles.size() * sizeof(PhysicsCircle));

        /// Draw
        window.Clear();

        uint32_t colorIndex = 0;
        for(auto i : circles)
        {
            window.DrawCircle(i.pos, i.radius, circleColors.at(colorIndex));
            colorIndex++;
        }

        window.DrawText("FPS: " + std::to_string(int(1.0 / delta)), {0,0}, 30);
        window.DrawText("Circles: " + std::to_string(CIRCLE_COUNT), {0,30}, 30);

        if(Mouse::ButtonDown(Mouse::Left))
            window.DrawText("Attracting", {0, 60}, 30, Color::Green);
        else
            window.DrawText("Repulsing", {0, 60}, 30, Color::Red);

        window.Display();
    }
}