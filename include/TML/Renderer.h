#pragma once
#include "Window.h"
#include "Drawable/Rectangle.h"
#include "Drawable/Circle.h"
#include "Drawable/Sprite.h"
#include "Drawable/Text.h"
#include "Drawable/Video.h"
#include "Camera.h"

namespace tml {
    class Renderer {
    public:
        static bool Init();
        static void SetClearColor(const Color& color);
        static void SetCamera(Camera& cam);
        static void ResetCamera();
        static void Clear();
        static void BeginBatch(); // Maybe make this private?
        static void Draw(Rectangle &rectangle);
        static void Draw(Circle &circle);
        static void Draw(Sprite &sprite);
        static void Draw(Text &text);

        // @brief You need to call video.Advance() before drawing
        static void Draw(Video& video);
        static void DrawLine(const Vector2 &a, const Vector2 &b, float thickness, Color color, bool rounded = true);
        static void DrawRect(const Vector2 &pos, const Vector2 &dimensions, const Color &color, float roundness = 0.0f,
                             float rotation = 0.0f);

        static void DrawCircle(const Vector2 &pos, float radius, const Color &color);

        // @brief Draws a cubic bezier-curve.
        // a and b are the end points, while cp1 and cp2 are the control points.
        static void
        DrawBezier(const Vector2 &a, const Vector2 &cp1, const Vector2 &b, const Vector2 &cp2, float thickness,
                   const Color &color, bool rounded = true, float step = 0.01f);

        // @brief Draws a quadratic bezier-curve.
        // a and b are the end points, while cp is the control point.
        static void
        DrawBezier(const Vector2 &a, const Vector2 &cp, const Vector2 &b, float thickness, const Color &color,
                   bool rounded = true, float step = 0.01f);
        // @brief Draws a grid. The rows and columns mean how many cells the grid should contain horizontally and vertically.
        static void DrawGrid(const Vector2& top_left, const Vector2& size, ui32 rows, ui32 columns,
                             const Color& color, float thickness = 1.f, bool rounded = false);
        static void DrawTexture(Texture& tex, const Vector2& pos, const Vector2& size);
        static void DrawText(const std::string& text, const Vector2& pos, float size, const Color& color = WHITE);
        static void EndBatch();
        static int batch_count;

    private:
        static void
        PushQuad(const Vector2 &pos, const Vector2 &size, const Color &col, Texture& tex_id, Vertex::Drawable_Type type);

        static void // Rotated
        PushQuad(const Vector2 &pos, const Vector2 &size, const Color &col, Texture& tex_id, float rotation, Vertex::Drawable_Type type);

        static void
        p_DrawRect(const Vector2 &pos, const Vector2 &dimensions, const Color &color, float rotation = 0.0f);

        static ui32 PushTexture(Texture& tex);
    };
};