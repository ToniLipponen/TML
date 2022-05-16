#include <TML/Graphics/RenderTarget.h>
#include "Renderer.h"

namespace tml
{
    static constexpr void MakeCircle(Image& image, uint32_t resolution) noexcept
    {
        auto* buffer = image.GetData();
        const auto radius = resolution / 2.0;
        const auto center = Vector2f(radius);

        for(uint32_t i = 0; i < resolution; ++i)
        {
            for(uint32_t j = 0; j < resolution; ++j)
            {
                const double dist = Math::Distance(Vector2f(j, i), center);
                const double d = dist / radius;
                buffer[i * resolution + j] = static_cast<unsigned char>(Math::SmoothStep(0.0, 0.002, 1.0 - d) * 255.0);
            }
        }
    }

    std::unique_ptr<Texture> RenderTarget::s_circleTexture = nullptr;

    RenderTarget::RenderTarget()
    : m_renderer(Renderer::GetInstance())
    {
        m_renderer.GetOpenGLVersion(m_version.major, m_version.minor);

        if(!s_circleTexture)
        {
            Image circleImage;
            circleImage.LoadFromMemory(2048, 2048, 1, nullptr);
            MakeCircle(circleImage, 2048);

            s_circleTexture = std::make_unique<Texture>();
            s_circleTexture->LoadFromImage(circleImage);
        }
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
        drawable.OnDraw(this, s_circleTexture.get());
    }

    void RenderTarget::DrawLine(const Vector2f &a, const Vector2f &b, float thickness, Color color, bool rounded) noexcept
    {
        static Line line;
        line.SetPointA(a);
        line.SetPointB(b);
        line.SetThickness(thickness);
        line.SetColor(color);
        line.SetRounded(rounded);

        Draw(line);
    }

    void RenderTarget::DrawRect(const Vector2f &pos, const Vector2f &dimensions, const Color &color, float roundness,
                                float rotation, const Vector2f& origin, bool applyOriginToPos) noexcept
    {
        static Rectangle rectangle;
        rectangle.SetPosition(pos);
        rectangle.SetSize(dimensions);
        rectangle.SetColor(color);
        rectangle.SetCornerRadius(roundness);
        rectangle.SetRotation(rotation);
        rectangle.SetOrigin(origin);
        rectangle.ApplyOriginToPosition(applyOriginToPos);

        Draw(rectangle);
    }

    void RenderTarget::DrawCircle(const Vector2f &pos, float radius, const Color &color) noexcept
    {
        PushQuad(pos-Vector2f(radius), Vector2f(radius*2), color, *s_circleTexture, Vertex::TEXT);
    }

    void RenderTarget::DrawBezier(const Vector2f &a, const Vector2f &cp1, const Vector2f& cp2, const Vector2f &b, float thickness,
                                  const Color &color, bool rounded, float step) noexcept
    {
        Vector2f begin = a;

        for(float i = 0; i < 1;)
        {
            const Vector2f end = Math::Cubic(a,cp1,cp2,b,i);
            DrawLine(begin, end, thickness, color, rounded);
            begin = end;
            i += step;
        }
    }

    void RenderTarget::DrawBezier(const Vector2f &a, const Vector2f &cp, const Vector2f &b, float thickness,
                                  const Color &color, bool rounded, float step) noexcept
    {
        Vector2f begin = a;

        for(float i = 0; i < 1;)
        {
            const Vector2f end = Math::Quadratic(a,cp,b,i);
            DrawLine(begin, end, thickness, color, rounded);
            begin = end;
            i += step;
        }
    }

    void RenderTarget::DrawGrid(const Vector2f &top_left, const Vector2f &size, uint32_t rows, uint32_t columns,
                                const Color &color, float thickness, bool rounded) noexcept
    {
        /** This is to prevent holes in corners when rounded == false and thickness > 1; */
        const auto offset = rounded ? 0 : thickness;

        for(uint32_t i = 0; i <= rows; ++i)
        {
            DrawLine(top_left + Vector2f{-offset,    (size.y / rows) * i},
                     top_left + Vector2f{offset + size.x, (size.y / rows) * i}, thickness, color, false);
        }
        for(uint32_t i = 0; i <= columns; ++i)
        {
            DrawLine(top_left + Vector2f{(size.x / columns) * i, 0.f},
                     top_left + Vector2f{(size.x / columns) * i, size.y}, thickness, color, rounded && (i == 0 || i == columns));
        }
    }

    void RenderTarget::DrawTexture(const Texture& tex, const Vector2f& pos, const Vector2f& size) noexcept
    {
        PushQuad(pos, size, Color::Transparent, tex, Vertex::TEXTURE);
    }

    void RenderTarget::DrawTextureRect(const Texture& tex, const Vector2f& pos, const Vector2f& size, float rotation,
                                       const Vector2f& tl, const Vector2f& br) noexcept
    {
        PushQuad(pos, size, Color::Transparent, tex, Vertex::TEXTURE, rotation, tl, br);
    }

    void RenderTarget::DrawText(const String& string, const Vector2f& pos, float size, const Color& color) noexcept
    {
        static Text text;
        text.SetString(string);
        text.SetSize(size);
        text.SetColor(color);
        text.SetPosition(pos);
        Draw(text);
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