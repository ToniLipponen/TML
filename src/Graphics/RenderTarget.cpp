#include <TML/Graphics/RenderTarget.h>
#include "Renderer.h"

namespace tml
{
    RenderTarget::RenderTarget()
    : m_renderer(Renderer::GetInstance())
    {

    }

    Vector2f RenderTarget::WorldToScreen(const Vector2f& point, const Camera& camera) const noexcept
    {
        return camera.WorldToScreen(point, GetRenderTargetSize());
    }

    Vector2f RenderTarget::ScreenToWorld(const Vector2f& point, const Camera& camera) const noexcept
    {
        return camera.ScreenToWorld(point, GetRenderTargetSize());
    }

    void RenderTarget::SetCamera(const Camera& camera)
    {
        m_renderer.SetCamera(camera);
    }

    void RenderTarget::ResetCamera()
    {
        m_renderer.ResetCamera();
    }

    void RenderTarget::SetBounds(const Vector2i &pos, const Vector2i &size) noexcept
    {
        m_renderer.SetBounds(pos, size);
    }

    void RenderTarget::SetViewport(const Vector2i &pos, const Vector2i &size) noexcept
    {
        const auto targetSize = static_cast<Vector2i>(GetRenderTargetSize());
        m_renderer.SetViewport(Vector2i(pos.x, -pos.y + targetSize.y - size.y), size);
    }

    void RenderTarget::SetView(const Vector2i &pos, const Vector2i &size) noexcept
    {
        m_renderer.SetView(pos, size);
    }

    void RenderTarget::ResetBounds() noexcept
    {
        m_renderer.ResetBounds();
    }

    void RenderTarget::SetClearColor(const Color& color) noexcept
    {
        m_clearColor[0] = ((float)color.r) / 255.0f;
        m_clearColor[1] = ((float)color.g) / 255.0f;
        m_clearColor[2] = ((float)color.b) / 255.0f;
        m_clearColor[3] = ((float)color.a) / 255.0f;
    }

    void RenderTarget::Clear() noexcept
    {
        m_renderer.Clear(m_clearColor);
    }

    void RenderTarget::Draw(Drawable& drawable) noexcept
    {
        m_renderer.Draw(drawable);
    }

    void RenderTarget::DrawLine(const Vector2f &a, const Vector2f &b, float thickness, Color color,
                                bool rounded) noexcept
    {
        m_renderer.DrawLine(a, b, thickness, color, rounded);
    }

    void RenderTarget::DrawRect(const Vector2f &pos, const Vector2f &dimensions, const Color &color, float roundness,
                                float rotation) noexcept
    {
        m_renderer.DrawRect(pos, dimensions, color, roundness, rotation);
    }

    void RenderTarget::DrawCircle(const Vector2f &pos, float radius, const Color &color) noexcept
    {
        m_renderer.DrawCircle(pos, radius, color);
    }

    void RenderTarget::DrawBezier(const Vector2f &a, const Vector2f &cp1, const Vector2f& cp2, const Vector2f &b, float thickness,
                                  const Color &color, bool rounded, float step) noexcept
    {
        m_renderer.DrawBezier(a, cp1, cp2, b, thickness, color, rounded, step);
    }

    void RenderTarget::DrawBezier(const Vector2f &a, const Vector2f &cp, const Vector2f &b, float thickness,
                                  const Color &color, bool rounded, float step) noexcept
    {
        m_renderer.DrawBezier(a, cp, b, thickness, color, rounded, step);
    }

    void RenderTarget::DrawGrid(const Vector2f &top_left, const Vector2f &size, uint32_t rows, uint32_t columns,
                                const Color &color, float thickness, bool rounded) noexcept
    {
        m_renderer.DrawGrid(top_left, size, rows, columns, color, thickness, rounded);
    }

    void RenderTarget::DrawTexture(const Texture& tex, const Vector2f& pos, const Vector2f& size) noexcept
    {
        m_renderer.DrawTexture(tex, pos, size);
    }

    void RenderTarget::DrawTextureRect(const Texture& tex, const Vector2f& pos, const Vector2f& size, float rotation,
                                       const Vector2f& tl, const Vector2f& br) noexcept
    {
        m_renderer.DrawTextureRect(tex, pos, size, rotation, tl, br);
    }

    void RenderTarget::DrawText(const String& text, const Vector2f& pos, float size, const Color& color) noexcept
    {
        m_renderer.DrawText(text, pos, size, color);
    }

    void RenderTarget::PushVertexData(const std::vector<Vertex> &vertices,
                                      const std::vector<uint32_t> &indices) noexcept
    {
        m_renderer.PushVertexData(vertices, indices);
    }

    void RenderTarget::PushVertexData(std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices,
                                      const Texture &texture) noexcept
    {
        m_renderer.PushVertexData(vertices, indices, texture);
    }

    uint32_t RenderTarget::PushTexture(const Texture& texture) noexcept
    {
        return m_renderer.PushTexture(texture);
    }

    void RenderTarget::PushQuad(const Vector2f &pos, const Vector2f &size, const Color &color, const Texture &texture,
                                Vertex::DrawableType type, float rotation, const Vector2f &uvTopLeft,
                                const Vector2f &uvBottomRight) noexcept
    {
        m_renderer.PushQuad(pos, size, color, texture, type, rotation, uvTopLeft, uvBottomRight);
    }

    void RenderTarget::EndBatch() noexcept
    {
        m_renderer.EndBatch();
    }
}