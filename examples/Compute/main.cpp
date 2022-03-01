#include <TML/Graphics.h>
#include <TML/System.h>
using namespace tml;

struct PhysicsData
{
    std::vector<Vector2f> positions;
    std::vector<Vector2f> velocities;
    std::vector<float> radii;
};

const char* shaderSrc = R"END(
#version 430 core

layout( local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout( std430, binding=1 ) buffer pBuff { vec2 Position[]; };
layout( std430, binding=2 ) buffer vBuff { vec2 Momentum[]; };
layout( std430, binding=3 ) buffer rBuff { float Radius[];  };

uniform float delta;
uniform vec2 screenSize;
uniform vec2 mousePos;
uniform bool mouseDown;

void main (void)
{
    uint i = gl_GlobalInvocationID.x;

    vec2 momentum = Momentum[i];
    float r = Radius[i];
    float dist = distance(Position[i], mousePos);

    if(mouseDown)
        Momentum[i] -= ((Position[i] - mousePos) / (dist*dist)) * 50;
    else
        Momentum[i] += ((Position[i] - mousePos) / (dist*dist)) * 50;

    if(Position[i].x - r < 0)
    {
        Momentum[i].x = -momentum.x * 0.9;
        Position[i].x = r;
    }
    else if(Position[i].x + r > screenSize.x)
    {
        Momentum[i].x = -momentum.x * 0.9;
        Position[i].x = screenSize.x - r;
    }

    if(Position[i].y - r < 0)
    {
        Momentum[i].y = -momentum.y * 0.9;
        Position[i].y = r;
    }
    else if(Position[i].y + r > screenSize.y)
    {
        Momentum[i].y = -momentum.y * 0.9;
        Position[i].y = screenSize.y - r;
    }
    Momentum[i] *= 1.0 - (delta / 2);
    Position[i] += Momentum[i] * delta;
}
)END";

int main()
{
    RenderWindow window(800, 600, "Compute", Window::Resizeable);

    std::vector<Color> circleColors;
    PhysicsData circles;
    for(auto i = 0; i < 5000; i++)
    {
        circles.positions.emplace_back(rand() % 800, rand() % 600);
        circles.velocities.emplace_back((rand() % 500) - 250);
        circles.radii.push_back(float(rand() % 20));
        circleColors.emplace_back(rand() % 255, rand() % 255, rand() % 255);
        circleColors.at(circleColors.size()-1).a = 255;
    }
    StorageBuffer posData, momentData, radData;
    posData.BufferData(circles.positions.data(), circles.positions.size() * sizeof(Vector2f));
    momentData.BufferData(circles.velocities.data(), circles.velocities.size() * sizeof(Vector2f));
    radData.BufferData(circles.radii.data(), circles.radii.size() * sizeof(float));

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

        /// Update storage buffers & connect them to the compute shader.
        posData.UpdateData(circles.positions.data(), circles.positions.size() * sizeof(Vector2f));
        shader.ConnectBuffer("pBuff", 1, posData);

        momentData.UpdateData(circles.velocities.data(), circles.velocities.size() * sizeof(Vector2f));
        shader.ConnectBuffer("vBuff", 2, momentData);

        radData.UpdateData(circles.radii.data(), circles.radii.size() * sizeof(float));
        shader.ConnectBuffer("rBuff", 3, radData);

        /// Set uniforms.
        shader.Uniform1f("delta", delta);
        shader.Uniform2f("screenSize", window.GetWidth(), window.GetHeight());
        shader.Uniform2f("mousePos", mousePos.x, mousePos.y);
        shader.Uniform1i("mouseDown", Mouse::ButtonDown(Mouse::Left));

        /// Execute.
        shader.Dispatch(circles.positions.size(), 1);
        shader.Wait();

        /// Load data from the storage buffers back to circles data buffers.
        posData.RetrieveData(&circles.positions[0], circles.positions.size() * sizeof(Vector2f));
        momentData.RetrieveData(&circles.velocities[0], circles.velocities.size() * sizeof(Vector2f));
        radData.RetrieveData(&circles.radii[0], circles.radii.size() * sizeof(float));

        window.Clear();
        for(auto i = 0; i < circles.positions.size(); ++i)
        {
            auto p = circles.positions.at(i);
            auto r = circles.radii.at(i);
            window.DrawCircle(p, r, circleColors.at(i));
        }
        window.DrawText("FPS: " + std::to_string(int(1.0 / delta)), {0,0}, 30);
        window.Display();
    }
}