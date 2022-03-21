#include <TML/Graphics/Drawable/Sprite.h>
#include <TML/Graphics/Renderer.h>
#include <TML/System/Math.h>

namespace tml
{
    Sprite::Sprite()
    : m_rect({{0, 0}, {0, 0}}), m_texSize(0)
    {
        m_pos = Vector2f{0,0};
        m_size = Vector2f{0,0};
        m_indexData = {0,1,2, 1,3,2};
        m_updated = true;
    }

    bool Sprite::LoadFromFile(const String& filename) noexcept
    {
        if(!m_img.LoadFromFile(filename))
            return false;

        m_size = Vector2f(m_img.GetWidth(), m_img.GetHeight());
        m_texSize = m_size;
        m_rect = {{0,0}, m_size};
        m_tex.LoadFromMemory(m_img.GetWidth(), m_img.GetHeight(), m_img.GetBpp(), m_img.GetData());
        m_updated = true;
        return true;
    }

    bool Sprite::LoadFromImage(const Image& image) noexcept
    {
        if(image.GetData() == nullptr)
            return false;

        m_img = image;
        m_size = Vector2f(m_img.GetWidth(), m_img.GetHeight());
        m_texSize = m_size;
        m_rect = {{0,0}, m_size};
        m_tex.LoadFromMemory(m_img.GetWidth(), m_img.GetHeight(), m_img.GetBpp(), m_img.GetData());
        m_updated = true;
        return true;
    }

    void Sprite::SetInterpolation(bool interpolate) noexcept
    {
        m_tex.SetMinMagFilter(
                interpolate ? Texture::LinearMipmapLinear : Texture::Nearest,
                interpolate ? Texture::LinearMipmapLinear : Texture::Nearest
        );
        m_tex.LoadFromMemory(m_img.GetWidth(), m_img.GetHeight(), m_img.GetBpp(), m_img.GetData());
    }

    void Sprite::SetSharedTexture(const std::shared_ptr<Texture>& texture) noexcept
    {
        m_sharedResource = texture;
        m_size = Vector2f(texture->GetWidth(), texture->GetHeight());
        m_texSize = m_size;
        m_rect = {{0,0}, m_size};
        m_updated = true;
    }

    void Sprite::SetRect(const TexRect& r) noexcept
    {
        m_rect = r;
        m_updated = true;
    }

    TexRect Sprite::GetRect() const noexcept
    {
        return m_rect;
    }

    Vector2f Sprite::GetTextureSize() const noexcept
    {
        if(m_sharedResource)
        {
            return {m_sharedResource->GetWidth(), m_sharedResource->GetHeight()};
        }
        return m_texSize;
    }

    void Sprite::OnDraw(class Renderer* renderer, Texture*) noexcept
    {
        if(m_updated)
        {
            m_vertexData.clear();

            const Vector2f tl = m_rect.pos / m_texSize;
            const Vector2f br = (m_rect.pos + m_rect.size) / m_texSize;

            if(m_rotation != 0)
            {
                m_vertexData.push_back(Vertex{Math::Rotate(m_pos + m_origin, m_pos, cos_r, sin_r),                           tl,           0x0, Vertex::TEXTURE});
                m_vertexData.push_back(Vertex{Math::Rotate(m_pos + m_origin, m_pos + Vector2f(m_size.x, 0.f), cos_r, sin_r), {br.x, tl.y}, 0x0, Vertex::TEXTURE});
                m_vertexData.push_back(Vertex{Math::Rotate(m_pos + m_origin, m_pos + Vector2f(0.f, m_size.y), cos_r, sin_r), {tl.x, br.y}, 0x0, Vertex::TEXTURE});
                m_vertexData.push_back(Vertex{Math::Rotate(m_pos + m_origin, m_pos + m_size, cos_r, sin_r),                  br,           0x0, Vertex::TEXTURE});
            }
            else
            {
                m_vertexData.push_back(Vertex{m_pos,                           tl,           0x0, Vertex::TEXTURE});
                m_vertexData.push_back(Vertex{m_pos + Vector2f(m_size.x, 0.f), {br.x, tl.y}, 0x0, Vertex::TEXTURE});
                m_vertexData.push_back(Vertex{m_pos + Vector2f(0.f, m_size.y), {tl.x, br.y}, 0x0, Vertex::TEXTURE});
                m_vertexData.push_back(Vertex{m_pos + m_size,                  br,           0x0, Vertex::TEXTURE});
            }
            m_updated = false;
        }
        renderer->PushVertexData(m_vertexData, m_indexData, m_sharedResource ? *m_sharedResource : m_tex);
    }
}