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

    bool Sprite::LoadFromFile(const String& filename)
    {
        if(!m_img.LoadFromFile(filename))
            return false;
        m_size = Vector2f(m_img.GetWidth(), m_img.GetHeight());
        m_texSize = m_size;
        m_rect = {{0,0}, m_size};
        m_tex.LoadFromMemory(m_img.GetWidth(), m_img.GetHeight(), m_img.GetBpp(), m_img.GetData());
        Generate();
        return true;
    }

    bool Sprite::LoadFromImage(const Image& image)
    {
        if(image.GetData() == nullptr)
            return false;
        m_img = image;
        m_size = Vector2f(m_img.GetWidth(), m_img.GetHeight());
        m_texSize = m_size;
        m_rect = {{0,0}, m_size};
        m_tex.LoadFromMemory(m_img.GetWidth(), m_img.GetHeight(), m_img.GetBpp(), m_img.GetData());
        Generate();
        return true;
    }

    void Sprite::SetInterpolation(bool interpolate)
    {
        m_tex.SetMinMagFilter(
                interpolate ? Texture::LinearMipmapLinear : Texture::Nearest,
                interpolate ? Texture::LinearMipmapLinear : Texture::Nearest
        );
    }

    void Sprite::Generate() noexcept
    {
        m_vertexData.clear();
        m_indexData.clear();

        const Vector2f tl = m_rect.pos / m_texSize;
        const Vector2f br = (m_rect.pos + m_rect.size) / m_texSize;

        if(m_rotation != 0)
        {
            const float cos_r = std::cos(Math::DegToRad(m_rotation));
            const float sin_r = std::sin(Math::DegToRad(m_rotation));

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

        m_indexData.push_back(0);
        m_indexData.push_back(1);
        m_indexData.push_back(2);

        m_indexData.push_back(1);
        m_indexData.push_back(3);
        m_indexData.push_back(2);
    }

}