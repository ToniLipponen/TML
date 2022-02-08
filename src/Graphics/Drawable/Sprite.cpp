#include <TML/Graphics/Drawable/Sprite.h>
#include "TML/System/Math.h"

namespace tml
{
    Sprite::Sprite()
    : m_rect({{0, 0}, {0, 0}}), m_texSize(0)
    {
        m_pos = Vector2f{0,0};
        m_size = Vector2f{0,0};
        Generate();
    }

    void Sprite::SetRect(const TexRect& r)
    {
        m_rect = r;
        Generate();
    }

    void Sprite::LoadFromFile(const std::string& filename)
    {
        m_img.LoadFromFile(filename);
        m_size = Vector2f(m_img.GetWidth(), m_img.GetHeight());
        m_texSize = m_size;
        m_rect = {{0,0}, m_size};
        m_tex.LoadFromMemory(m_img.GetWidth(), m_img.GetHeight(), m_img.GetBpp(), m_img.GetData());
        Generate();
    }

    void Sprite::LoadFromImage(const Image& image)
    {
        m_img = image;
        m_size = Vector2f(m_img.GetWidth(), m_img.GetHeight());
        m_texSize = m_size;
        m_rect = {{0,0}, m_size};
        m_tex.LoadFromMemory(m_img.GetWidth(), m_img.GetHeight(), m_img.GetBpp(), m_img.GetData());
        Generate();
    }

    void Sprite::SetInterpolation(bool interpolate)
    {
        m_tex.SetMinMagFilter(
                interpolate ? Texture::Linear : Texture::Nearest,
                interpolate ? Texture::Linear : Texture::Nearest
        );
    }

    void Sprite::Generate() noexcept
    {
        m_vertexData.clear();
        m_indexData.clear();

        const Vector2f tl = m_rect.pos / m_texSize;
        const Vector2f br = (m_rect.pos + m_rect.size) / m_texSize;
        const Vector2f origin = (m_pos + m_pos + m_size) * 0.5f;
        m_vertexData.push_back(Vertex{Math::Rotate(origin, m_pos, m_rotation),                           tl,           0x0, 0, Vertex::TEXTURE});
        m_vertexData.push_back(Vertex{Math::Rotate(origin, m_pos + Vector2f(m_size.x, 0.f), m_rotation), {br.x, tl.y}, 0x0, 0, Vertex::TEXTURE});
        m_vertexData.push_back(Vertex{Math::Rotate(origin, m_pos + Vector2f(0.f, m_size.y), m_rotation), {tl.x, br.y}, 0x0, 0, Vertex::TEXTURE});
        m_vertexData.push_back(Vertex{Math::Rotate(origin, m_pos + m_size, m_rotation),                  br,           0x0, 0, Vertex::TEXTURE});

        m_indexData.push_back(0);
        m_indexData.push_back(1);
        m_indexData.push_back(2);

        m_indexData.push_back(1);
        m_indexData.push_back(3);
        m_indexData.push_back(2);
    }

}