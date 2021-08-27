#pragma once
#include "Window.h"
#include "Shader.h"
#include "Drawable/Rectangle.h"
#include "Drawable/Circle.h"
#include "Drawable/Sprite.h"
#include "Drawable/Text.h"

class Renderer
{
public:
    static void Init();
    static void Clear();
    static void BeginBatch();
    static void Draw(Rectangle& rectangle);
    static void Draw(Circle& circle);
    static void Draw(Sprite& sprite);
    static void Draw(Text& text);

    static void DrawLine(const Vector2& a, const Vector2& b, float thickness, Color color, bool rounded = true);
    static void DrawRect(const Vector2& pos, const Vector2& dimensions, const Color& color, float roundness = 0.0f, float rotation = 0.0f);
    static void DrawCircle(const Vector2& pos, float radius, Color color);
    static void DrawText(const std::string& text, Font& font, Color color);

    static void EndBatch();
private:
    static void p_DrawRect(const Vector2& pos, const Vector2& dimensions, const Color& color, float rotation = 0.0f);
};