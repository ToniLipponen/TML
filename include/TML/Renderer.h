#pragma once
#include "Window.h"
#include "Drawable/Rectangle.h"
#include "Drawable/Circle.h"
#include "Drawable/Sprite.h"
#include "Drawable/Text.h"
#include "Drawable/Video.h"
#include "Camera.h"

namespace tml
{
    class Renderer
    {
    public:
        static bool Init() noexcept;
        static bool QuietInit() noexcept;

        static void SetClearColor(const Color& color) noexcept;
        static void SetCamera(Camera& cam) noexcept;
        static void ResetCamera() noexcept;

        static void SetViewport(i32 x, i32 y, ui32 w, ui32 h) noexcept;
        static void SetViewport(const Vector2& pos, const Vector2& size) noexcept;
        static void ResetViewport() noexcept;

        static void SetBounds(const Vector2& pos, const Vector2& size) noexcept;
        static void ResetBounds() noexcept;

        static void Clear() noexcept;

        static void Draw(Rectangle &rectangle) noexcept;
        static void Draw(Circle &circle) noexcept;
        static void Draw(Sprite &sprite) noexcept;
        static void Draw(Text &text) noexcept;
        static void Draw(Video& video) noexcept; // @brief You need to call video.Advance() before drawing

        static void DrawLine(const Vector2 &a, const Vector2 &b, float thickness, Color color, bool rounded = true) noexcept;
        static void DrawRect(const Vector2 &pos, const Vector2 &dimensions, const Color &color, float roundness = 0.0f,
                             float rotation = 0.0f) noexcept;

        static void DrawCircle(const Vector2 &pos, float radius, const Color &color) noexcept;

        // @brief Draws a cubic bezier-curve.
        // a and b are the end points, while cp1 and cp2 are the control points.
        static void
        DrawBezier(const Vector2 &a, const Vector2 &cp1, const Vector2 &cp2, const Vector2 &b, float thickness,
                   const Color &color, bool rounded = true, float step = 0.01f) noexcept;

        // @brief Draws a quadratic bezier-curve.
        // a and b are the end points, while cp is the control point.
        static void
        DrawBezier(const Vector2 &a, const Vector2 &cp, const Vector2 &b, float thickness, const Color &color,
                   bool rounded = true, float step = 0.01f) noexcept;

        // @brief Draws a grid. The rows and columns mean how many cells the grid should contain horizontally and vertically.
        static void DrawGrid(const Vector2& top_left, const Vector2& size, ui32 rows, ui32 columns,
                             const Color& color, float thickness = 1.f, bool rounded = false) noexcept;

        static void DrawTexture(Texture& tex, const Vector2& pos, const Vector2& size) noexcept;

        static void
        DrawText(const std::string& text, const Vector2& pos, float size, const Color& color = WHITE) noexcept;

        static void EndBatch() noexcept;

    private:
        static void BeginBatch() noexcept; // Maybe make this private?
        static void
        PushQuad(const Vector2 &pos, const Vector2 &size, const Color &col, Texture& tex_id, Vertex::DrawableType type) noexcept;

        static void // Rotated
        PushQuad(const Vector2 &pos, const Vector2 &size, const Color &col, Texture& tex_id, float rotation, Vertex::DrawableType type) noexcept;

        static ui32 PushTexture(Texture& tex) noexcept;
        static void PushVertexData(std::vector<Vertex>& vertices, std::vector<ui32>& indices) noexcept;
    };
}