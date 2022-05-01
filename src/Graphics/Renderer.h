#pragma once
#include <TML/Graphics/Drawable/Rectangle.h>
#include <TML/Graphics/Drawable/Circle.h>
#include <TML/Graphics/Drawable/Sprite.h>
#include <TML/Graphics/Drawable/Text.h>
#include <TML/Graphics/Drawable/Shape.h>
#include <TML/Graphics/Drawable/Line.h>
#include <TML/Graphics/Camera.h>

namespace tml
{
    class Renderer
    {
    private:
        Renderer();
    public:
        Renderer(const Renderer&)            = delete;
        Renderer(Renderer&&)                 = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&)      = delete;

        virtual ~Renderer();

        static Renderer& GetInstance() noexcept;

        void Reset() noexcept;

        void SetBounds(const Vector2i& pos, const Vector2i& size) noexcept;

        void SetViewport(const Vector2i& pos, const Vector2i& size) noexcept;

        void SetView(const Vector2i& pos, const Vector2i& size) noexcept;

        void ResetBounds() noexcept;

        void SetCamera(const Camera& camera) noexcept;

        void ResetCamera() noexcept;

        void SetClearColor(const Color& color) noexcept;

        void Clear() noexcept;

        void Clear(float color[4]) noexcept;

        void Draw(Drawable& drawable) noexcept;

        void DrawLine(const Vector2f& a, const Vector2f& b, float thickness, Color color, bool rounded = true) noexcept;

        void DrawRect(const Vector2f& pos, const Vector2f& dimensions, const Color& color, float roundness = 0.0f,
                     float rotation = 0.0f) noexcept;

        void DrawCircle(const Vector2f& pos, float radius, const Color& color) noexcept;

        void DrawBezier(const Vector2f& a, const Vector2f& cp1, const Vector2f& cp2, const Vector2f& b,
                       float thickness,  const Color& color, bool rounded = true, float step = 0.01f) noexcept;
        void DrawBezier(const Vector2f& a, const Vector2f& cp, const Vector2f& b, float thickness,
                               const Color& color, bool rounded = true, float step = 0.01f) noexcept;

        void DrawGrid(const Vector2f& top_left, const Vector2f& size, uint32_t rows, uint32_t columns,
                     const Color& color, float thickness = 1.f, bool rounded = false) noexcept;

        void DrawTexture(const Texture& tex, const Vector2f& pos, const Vector2f& size) noexcept;

        void DrawTextureRect(const Texture& tex, const Vector2f& pos, const Vector2f& size, float rotation, const Vector2f& tl, const Vector2f& br) noexcept;

        void DrawText(const String& text, const Vector2f& pos, float size, const Color& color = Color::White) noexcept;

        void PushVertexData(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) noexcept;

        void PushVertexData(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const Texture& texture) noexcept;

        uint32_t PushTexture(const Texture&) noexcept;

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

        void EndBatch() noexcept;

        void BeginBatch() noexcept;

    protected:
        class VertexArray*  m_vao          = nullptr;
        class VertexVector* m_vertexVector = nullptr;
        class IndexVector*  m_indexVector  = nullptr;
        class Shader*       m_shader       = nullptr;
        class BufferLayout* m_layout       = nullptr;

        float   m_view[16]{};
        float   m_proj[16]{};
        float   m_scale[16]{};
        float   m_clearColor[4]{};

        TexRect m_viewport;
        Text    m_text;
        Texture m_circleTexture;

        constexpr static uint32_t s_maxVertexCount = 10000;
        constexpr static uint32_t s_maxIndexCount = s_maxVertexCount * 4;
        int32_t m_maxTextureCount = 8;
        std::vector<uint32_t> m_textures;
    };
}