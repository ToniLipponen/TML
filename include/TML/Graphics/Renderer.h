#pragma once
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
        Renderer();
        virtual ~Renderer();
        /// Use Renderer::QuietInit() if you don't want GPU info in terminal on initialization.
        bool Init() noexcept;

        void SetClearColor(const Color& color) noexcept;
        void SetCamera(const Camera& cam) noexcept;
        void ResetCamera() noexcept;

        void SetBounds(const Vector2i& pos, const Vector2i& size) noexcept;
        void SetViewport(const Vector2i& pos, const Vector2i& size) noexcept;
        void ResetBounds() noexcept;

        virtual void Clear() noexcept;
        void Draw(const Rectangle& rectangle) noexcept;
        void Draw(Circle& circle) noexcept;
        void Draw(Sprite& sprite) noexcept;
        void Draw(Text& text) noexcept;
        void Draw(const Video& video) noexcept; /// @warning You need to call video.Advance() before drawing

        void DrawLine(const Vector2f& a, const Vector2f& b, float thickness, Color color, bool rounded = true) noexcept;
        void DrawRect(const Vector2f& pos, const Vector2f& dimensions, const Color& color, float roundness = 0.0f,
                             float rotation = 0.0f) noexcept;

        void DrawCircle(const Vector2f& pos, float radius, const Color& color) noexcept;

        /** @brief Draws a cubic bezier-curve, where a and b are the end points and cp1 and cp2 are the control points.
         * @param step Defines the smoothness of the curve, where a lower value means smoother curve.
         */
        void DrawBezier(const Vector2f& a, const Vector2f& cp1, const Vector2f& cp2, const Vector2f& b,
                               float thickness,  const Color& color, bool rounded = true, float step = 0.01f) noexcept;

        /** @brief Draws a quadratic bezier-curve, where a and b are the end points, while cp is the control point.
         * @param step Defines the smoothness of the curve, where a lower value means smoother curve.
         */
        void DrawBezier(const Vector2f& a, const Vector2f& cp, const Vector2f& b, float thickness,
                               const Color& color, bool rounded = true, float step = 0.01f) noexcept;

        /// @brief Draws a grid. The rows and columns mean how many cells the grid should contain horizontally and vertically.
        void DrawGrid(const Vector2f& top_left, const Vector2f& size, ui32 rows, ui32 columns,
                             const Color& color, float thickness = 1.f, bool rounded = false) noexcept;

        void DrawTexture(const Texture& tex, const Vector2f& pos, const Vector2f& size) noexcept;
        void DrawTextureRect(const Texture& tex, const Vector2f& pos, const Vector2f& size, float rotation, const Vector2f& tl, const Vector2f& br) noexcept;

        /** @Brief Draws string of text on pos with a given size.
         * @warning This is going to be very slow,
         * because the vertex data is going to have to be constructed on each draw.
         * If you are going to be drawing text that doesn't change each frame, you should use Renderer::Draw(Text& text) instead
        */
        void DrawText(const String& text, const Vector2f& pos, float size, const Color& color = Color::White) noexcept;
        void PushVertexData(const std::vector<Vertex>& vertices, const std::vector<ui32>& indices) noexcept;
        void PushVertexData(std::vector<Vertex>& vertices, const std::vector<ui32>& indices, const Texture& texture) noexcept;
    protected:
        void EndBatch(bool flip = false) noexcept;
        void BeginBatch() noexcept;
        void PushQuad(
                const Vector2f& pos,
                const Vector2f& size,
                const Color& color,
                const Texture& texture,
                Vertex::DrawableType type,
                float rotation = 0.0f,
                const Vector2f& uvTopLeft = {0.0f, 0.0f},
                const Vector2f& uvBottomRight = {1.0f, 1.0f}
                ) noexcept;
        ui32 PushTexture(const Texture&) noexcept;

    protected:
        class VertexArray*  m_vao          = nullptr;
        class VertexBuffer* m_vertexBuffer = nullptr;
        class IndexBuffer*  m_indexBuffer  = nullptr;
        class Shader*       m_shader       = nullptr;
        class BufferLayout* m_layout       = nullptr;

        float m_view[16];
        float m_proj[16];
        float m_scale[16];
        TexRect m_viewport;
        Text      m_text;
        Texture   m_circleTexture;

        constexpr static ui32 MAX_VERTEX_COUNT = 100000;
        i32 MAX_TEXTURE_COUNT = 8;

        std::vector<Vertex>  m_vertexData;
        std::vector<ui32>    m_indexData;
        std::vector<ui32>    m_textures;
    };
}