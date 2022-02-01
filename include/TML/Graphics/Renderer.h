#pragma once
#include <TML/Graphics/Window.h>
#include <TML/Graphics/Camera.h>
#include <TML/Graphics/Drawable/Rectangle.h>
#include <TML/Graphics/Drawable/Circle.h>
#include <TML/Graphics/Drawable/Sprite.h>
#include <TML/Graphics/Drawable/Text.h>
#include <TML/Graphics/Drawable/Video.h>

namespace tml
{
    class Renderer
    {
    public:
        /// Use @b Renderer::QuietInit() if you don't want GPU info in terminal on initialization.
        static bool Init() noexcept;
        static bool QuietInit() noexcept;
        static void Deinit() noexcept;

        static void SetClearColor(const Color& color) noexcept;
        static void SetCamera(Camera& cam) noexcept;
        static void ResetCamera() noexcept;

        static void SetBounds(const Vector2i& pos, const Vector2i& size) noexcept;
        static void ResetBounds() noexcept;

        static void Clear() noexcept;
        static void Draw(Rectangle& rectangle) noexcept;
        static void Draw(Circle& circle) noexcept;
        static void Draw(Sprite& sprite) noexcept;
        static void Draw(Text& text) noexcept;
        static void Draw(Video& video) noexcept; /// @warning You need to call video.Advance() before drawing

        static void DrawLine(const Vector2f& a, const Vector2f& b, float thickness, Color color, bool rounded = true) noexcept;
        static void DrawRect(const Vector2f& pos, const Vector2f& dimensions, const Color& color, float roundness = 0.0f,
                             float rotation = 0.0f) noexcept;

        static void DrawCircle(const Vector2f& pos, float radius, const Color& color) noexcept;

        /** @brief Draws a cubic bezier-curve, where a and b are the end points and cp1 and cp2 are the control points.
         * @param step Defines the smoothness of the curve, where a lower value means smoother curve.
         */
         static void DrawBezier(const Vector2f& a, const Vector2f& cp1, const Vector2f& cp2, const Vector2f& b,
                               float thickness,  const Color& color, bool rounded = true, float step = 0.01f) noexcept;

        /** @brief Draws a quadratic bezier-curve, where a and b are the end points, while cp is the control point.
         * @param step Defines the smoothness of the curve, where a lower value means smoother curve.
         */
        static void DrawBezier(const Vector2f& a, const Vector2f& cp, const Vector2f& b, float thickness,
                               const Color& color, bool rounded = true, float step = 0.01f) noexcept;

        /// @brief Draws a grid. The rows and columns mean how many cells the grid should contain horizontally and vertically.
        static void DrawGrid(const Vector2f& top_left, const Vector2f& size, ui32 rows, ui32 columns,
                             const Color& color, float thickness = 1.f, bool rounded = false) noexcept;

        static void DrawTexture(Texture& tex, const Vector2f& pos, const Vector2f& size) noexcept;
        static void DrawTextureRect(Texture& tex, const Vector2f& pos, const Vector2f& size, float rotation, const Vector2f& tl, const Vector2f& br) noexcept;

        /** @Brief Draws string of text on pos with a given size.
         * @warning This is going to be very slow,
         * because the vertex data is going to have to be constructed on each draw.
         * If you are going to be drawing text that doesn't change each frame, you should use Renderer::Draw(Text& text) instead
        */
        static void DrawText(const std::string& text, const Vector2f& pos, float size, const Color& color = WHITE) noexcept;
        static void PushVertexData(std::vector<Vertex>& vertices, std::vector<ui32>& indices) noexcept;
        static void PushVertexData(std::vector<Vertex>& vertices, std::vector<ui32>& indices, Texture& texture) noexcept;
        static void EndBatch() noexcept;
    private:
        static void BeginBatch() noexcept;
        static void PushQuad(
                const Vector2f& pos,
                const Vector2f& size,
                const Color& color,
                Texture& texture,
                Vertex::DrawableType type,
                float rotation = 0.0f,
                const Vector2f& uvTopLeft = {0.0f, 0.0f},
                const Vector2f& uvBottomRight = {1.0f, 1.0f}
                ) noexcept;
        static ui32 PushTexture(Texture&) noexcept;
    };
}