#pragma once
#include "Window.h"
#include "Shader.h"
#include "Drawable/Rectangle.h"
#include "Drawable/Circle.h"
#include "Drawable/Sprite.h"
#include "Drawable/Text.h"
#include "Drawable/Video.h"

namespace tml {
    class Renderer {
    public:
        static void Init();

        static void Clear();

        static void BeginBatch();

        static void Draw(Rectangle &rectangle);

        static void Draw(Circle &circle);

        static void Draw(Sprite &sprite);

        static void Draw(Text &text);

        // You need to call video.Advance() before drawing
        static void Draw(Video& video);

        static void DrawLine(const Vector2 &a, const Vector2 &b, float thickness, Color color, bool rounded = true);

        static void DrawRect(const Vector2 &pos, const Vector2 &dimensions, const Color &color, float roundness = 0.0f,
                             float rotation = 0.0f);

        static void DrawCircle(const Vector2 &pos, float radius, const Color &color);

        // Draws a cubic bezier-curve.
        // @a and @b are the end points, while @cp1 and @cp2 are the control points.
        static void
        DrawBezier(const Vector2 &a, const Vector2 &cp1, const Vector2 &b, const Vector2 &cp2, float thickness,
                   const Color &color, bool rounded = true, float step = 0.01f);

        // Draws a quadratic bezier-curve.
        // @a and @b are the end points, while @cp is the control point.
        static void
        DrawBezier(const Vector2 &a, const Vector2 &cp, const Vector2 &b, float thickness, const Color &color,
                   bool rounded = true, float step = 0.01f);

        static void EndBatch();

    private:
        static void
        p_DrawRect(const Vector2 &pos, const Vector2 &dimensions, const Color &color, float rotation = 0.0f);
    };
};