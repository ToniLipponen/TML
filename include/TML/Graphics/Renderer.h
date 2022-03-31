#pragma once
#include <TML/Graphics/Camera.h>
#include <TML/Graphics/Drawable/Rectangle.h>
#include <TML/Graphics/Drawable/Circle.h>
#include <TML/Graphics/Drawable/Sprite.h>
#include <TML/Graphics/Drawable/Text.h>
#include <TML/Graphics/Drawable/Video.h>
#include <TML/Graphics/Drawable/Shape.h>
#include <TML/Graphics/Drawable/Line.h>

namespace tml
{
    class Renderer
    {
    public:
        Renderer();
        virtual ~Renderer();

        void SetClearColor(const Color& color) noexcept;

        void SetCamera(const Camera& cam) noexcept;
        void SetBounds(const Vector2i& pos, const Vector2i& size) noexcept;
        void SetViewport(const Vector2i& pos, const Vector2i& size) noexcept;
        void SetView(const Vector2i& pos, const Vector2i& size) noexcept;

        void ResetCamera() noexcept;
        void ResetBounds() noexcept;

        virtual void Clear() noexcept;
        void Draw(Drawable& drawable) noexcept;

        void DrawLine(const Vector2f& a, const Vector2f& b, float thickness, Color color, bool rounded = true) noexcept;
        void DrawRect(const Vector2f& pos, const Vector2f& dimensions, const Color& color, float roundness = 0.0f,
                     float rotation = 0.0f) noexcept;

        void DrawCircle(const Vector2f& pos, float radius, const Color& color) noexcept;

        /** @brief Draws a cubic bezier-curve, where a and b are the end points and cp1 and cp2 are the control points.
         * @param step Defines the smoothness of the curve, where a lower value means smoother curve. */
        void DrawBezier(const Vector2f& a, const Vector2f& cp1, const Vector2f& cp2, const Vector2f& b,
                       float thickness,  const Color& color, bool rounded = true, float step = 0.01f) noexcept;

        /** @brief Draws a quadratic bezier-curve, where a and b are the end points, while cp is the control point.
         * @param step Defines the smoothness of the curve, where a lower value means smoother curve. */
        void DrawBezier(const Vector2f& a, const Vector2f& cp, const Vector2f& b, float thickness,
                               const Color& color, bool rounded = true, float step = 0.01f) noexcept;

        /// @brief Draws a grid. The rows and columns mean how many cells the grid should contain horizontally and vertically.
        void DrawGrid(const Vector2f& top_left, const Vector2f& size, ui32 rows, ui32 columns,
                     const Color& color, float thickness = 1.f, bool rounded = false) noexcept;

        /// @brief Does exactly what it says. Just draws a texture on pos with given size.
        void DrawTexture(const Texture& tex, const Vector2f& pos, const Vector2f& size) noexcept;

        /** @brief Draws texture with a given position and size. Rotation happens around the center of the texture. tl and rb are UV coordinates.
         *
         * @param tex Texture of your choosing
         * @param pos Position on the screen.
         * @param size Size on the screen.
         * @param rotation Rotation in degrees around the middle point.
         * @param tl Top left UV coordinate of the texture.
         * @param br Bottom right UV coordinate of the texture. */
        void DrawTextureRect(const Texture& tex, const Vector2f& pos, const Vector2f& size, float rotation, const Vector2f& tl, const Vector2f& br) noexcept;

        /** @Brief Draws string of text on pos with a given size.
         * @warning This is going to be very slow,
         * because the vertex data is going to have to be constructed on each draw.
         * If you are going to be drawing text that doesn't change each frame, you should use Renderer::Draw(Text& text) instead. */
        void DrawText(const String& text, const Vector2f& pos, float size, const Color& color = Color::White) noexcept;
        void PushVertexData(const std::vector<Vertex>& vertices, const std::vector<ui32>& indices) noexcept;
        void PushVertexData(std::vector<Vertex>& vertices, const std::vector<ui32>& indices, const Texture& texture) noexcept;
        ui32 PushTexture(const Texture&) noexcept;
        void PushQuad(
                const Vector2f& pos,
                const Vector2f& size,
                const Color& color,
                const Texture& texture,
                Vertex::DrawableType type,
                float rotation = 0.0f,
                const Vector2f& uvTopLeft = {0.0f, 1.0f},
                const Vector2f& uvBottomRight = {1.0f, 0.0f}
                ) noexcept;

    protected:
        void BeginBatch() noexcept;
        void EndBatch() noexcept;

    protected:
        class VertexArray*  m_vao          = nullptr;
        class VertexVector* m_vertexVector = nullptr;
        class IndexVector*  m_indexVector  = nullptr;
        class Shader*       m_shader       = nullptr;
        class BufferLayout* m_layout       = nullptr;

        float   m_view[16];
        float   m_proj[16];
        float   m_scale[16];
        TexRect m_viewport;
        Text    m_text;
        Texture m_circleTexture;
        float m_clearColor[4] = {0,0,0,0};

        constexpr static ui32 s_maxVertexCount = 1000000;
        constexpr static ui32 s_maxIndexCount = s_maxVertexCount * 1.5;
        i32 m_maxTextureCount = 8;
        std::vector<ui32> m_textures;
    };
}