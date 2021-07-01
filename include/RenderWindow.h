#pragma once
#include "Window.h"
#include "Shader.h"
#include "Drawable/Rectangle.h"
#include "Drawable/Circle.h"


class Renderer
{
public:
    static void Init();
    static void Clear();
    static void Draw(Rectangle& rectangle);
    static void Draw(Circle& circle);

    static void EndBatch();
private:
};
// class RenderWindow : public Window
// {
// public:
//     RenderWindow(ui32 w, ui32 h, cstring title, ui32 settings = 0);
//     void Clear();
//     void Draw(Rectangle& rectangle);
//     void Display() override;
// private:
//     void RenderBatch();
//     VertexArray     m_vao;
//     VertexBuffer    m_vertexBuffer;
//     IndexBuffer     m_indexBuffer;
//     BufferLayout    m_layout;

//     Shader m_shader;
// };