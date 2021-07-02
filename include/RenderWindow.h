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
    static void EndBatch();
private:
};