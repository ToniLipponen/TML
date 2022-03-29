#include <TML/Graphics/Drawable/Sprite.h>
#include <TML/Graphics/Renderer.h>
#include <TML/System/Math.h>

namespace tml
{
    Sprite::Sprite()
    : m_rect({{0, 0}, {0, 0}}), m_texSize(0), m_texture(nullptr)
    {
        m_pos = Vector2f{0,0};
        m_size = Vector2f{0,0};
        m_indexData = {0,1,2, 1,3,2};
        m_updated = true;
    }

    bool Sprite::LoadFromFile(const String& filename) noexcept
    {
        Image image;
        if(!image.LoadFromFile(filename))
            return false;
        image.FlipVertically();

        m_size = Vector2f(image.GetWidth(), image.GetHeight());
        m_texSize = m_size;
        m_rect = {{0,0}, m_size};
        m_texture = std::make_shared<Texture>();
        m_texture->LoadFromMemory(image.GetWidth(), image.GetHeight(), image.GetBpp(), image.GetData());
        m_updated = true;
        return true;
    }

    bool Sprite::LoadFromImage(const Image& image) noexcept
    {
        if(image.GetData() == nullptr)
            return false;

        auto newTexture = std::make_shared<Texture>();
        newTexture->LoadFromImage(image);
        return LoadFromTexture(newTexture);
    }

    bool Sprite::LoadFromTexture(const Texture& texture) noexcept
    {
        auto newTexture = std::make_shared<Texture>();
        *newTexture = texture;
        return LoadFromTexture(newTexture);
    }

    bool Sprite::LoadFromTexture(const std::shared_ptr<Texture>& texture) noexcept
    {
        m_texture = texture;
        if(m_texture == nullptr)
            return false;

        m_size = Vector2f(m_texture->GetWidth(), m_texture->GetHeight());
        m_texSize = m_size;
        m_rect = {{0,0}, m_size};
        m_updated = true;
        return (m_texture != nullptr) && (m_texture->GetID() != 0);
    }

    void Sprite::SetInterpolation(bool interpolate) noexcept
    {
        if(m_texture)
        {
            m_texture->SetMinMagFilter(
                    interpolate ? Texture::LinearMipmapLinear : Texture::Nearest,
                    interpolate ? Texture::LinearMipmapLinear : Texture::Nearest
            );
        }
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
        return m_texSize;
    }

    const std::shared_ptr<Texture>& Sprite::GetTexture() const noexcept
    {
        return m_texture;
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
                m_vertexData.push_back(Vertex{Math::Rotate(m_pos + m_origin, m_pos, cos_r, sin_r),                           {tl.x, br.y}, 0x0, Vertex::TEXTURE});
                m_vertexData.push_back(Vertex{Math::Rotate(m_pos + m_origin, m_pos + Vector2f(m_size.x, 0.f), cos_r, sin_r), {br.x, br.y}, 0x0, Vertex::TEXTURE});
                m_vertexData.push_back(Vertex{Math::Rotate(m_pos + m_origin, m_pos + Vector2f(0.f, m_size.y), cos_r, sin_r), {tl.x, tl.y}, 0x0, Vertex::TEXTURE});
                m_vertexData.push_back(Vertex{Math::Rotate(m_pos + m_origin, m_pos + m_size, cos_r, sin_r),                  {br.x, tl.y}, 0x0, Vertex::TEXTURE});
            }
            else
            {
                m_vertexData.push_back(Vertex{m_pos,                           {tl.x, br.y}, 0x0, Vertex::TEXTURE});
                m_vertexData.push_back(Vertex{m_pos + Vector2f(m_size.x, 0.f), {br.x, br.y}, 0x0, Vertex::TEXTURE});
                m_vertexData.push_back(Vertex{m_pos + Vector2f(0.f, m_size.y), {tl.x, tl.y}, 0x0, Vertex::TEXTURE});
                m_vertexData.push_back(Vertex{m_pos + m_size,                  {br.x, tl.y}, 0x0, Vertex::TEXTURE});
            }
            m_updated = false;
        }
        if(m_texture)
            renderer->PushVertexData(m_vertexData, m_indexData, *m_texture);
    }
}