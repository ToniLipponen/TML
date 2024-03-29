#pragma once
#include <TML/Graphics/Drawable/Rectangle.h>
#include <TML/Graphics/Drawable/Circle.h>
#include <TML/Graphics/Drawable/Sprite.h>
#include <TML/Graphics/Drawable/Text.h>
#include <TML/Graphics/Drawable/Shape.h>
#include <TML/Graphics/Drawable/Line.h>
#include <TML/Graphics/Camera.h>
#include <stack>

namespace tml
{
    struct IntRect
    {
        Vector2i pos, size;
    };

    struct RendererState
    {
        Matrix4f viewMatrix;
        Matrix4f projectionMatrix;
        Vector4f clearColor;
        IntRect viewport;
        IntRect viewOffsetAndDimensions;
        IntRect scissor;
    };

    class Renderer
    {
    private:
        Renderer();
        ~Renderer();
    public:
        Renderer(const Renderer&)            = delete;
        Renderer(Renderer&&)                 = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&)      = delete;

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
        void GetOpenGLVersion(int& major, int& minor) const noexcept;
		[[nodiscard]] const Matrix4f& GetViewMatrix() const noexcept;
        void PushState() noexcept;
        void PopState() noexcept;

    protected:
        [[nodiscard]] inline bool CheckLimits(uint32_t vertexCount, uint32_t indexCount, uint32_t textureCount) const noexcept;

    protected:
        class VertexArray*  m_vao          = nullptr;
        class VertexBuffer* m_vertexBuffer = nullptr;
        class IndexBuffer*  m_indexBuffer  = nullptr;
        class Shader*       m_shader       = nullptr;
        class BufferLayout* m_layout       = nullptr;

        std::stack<RendererState> m_statesStack;
        RendererState m_currentState;

        constexpr static uint32_t s_maxVertexCount = 10000;
        constexpr static uint32_t s_maxIndexCount = s_maxVertexCount * 4;

        int32_t m_maxTextureCount = 8;
        std::vector<uint32_t> m_textures;
        std::vector<Vertex> m_vertexData;
        std::vector<uint32_t> m_indexData;
    };
}