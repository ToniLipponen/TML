#include <TML/Graphics/Drawable/Sprite.h>
#include <TML/Graphics/RenderTarget.h>
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

    Sprite::Sprite(const Sprite& other) noexcept
	: Drawable(other)
	{
        this->m_pos             = other.m_pos;
        this->m_size            = other.m_size;
        this->m_texture         = other.m_texture;
        this->m_rect            = other.m_rect;
        this->m_texSize         = other.m_texSize;
        this->m_vertexData      = other.m_vertexData;
        this->m_origin          = other.m_origin;
        this->m_scale           = other.m_scale;
        this->m_rotation        = other.m_rotation;

        m_indexData = {0,1,2, 1,3,2};
    }

    Sprite::Sprite(Sprite&& other) noexcept
    {
        std::swap(m_pos, other.m_pos);
        std::swap(m_size, other.m_size);
        std::swap(m_rect, other.m_rect);
        std::swap(m_texSize, other.m_texSize);
        std::swap(m_vertexData, other.m_vertexData);
        std::swap(m_origin, other.m_origin);
        std::swap(m_scale, other.m_scale);
        std::swap(m_rotation, other.m_rotation);
        m_indexData = {0,1,2, 1,3,2};
        m_texture = std::move(other.m_texture);
    }

    Sprite& Sprite::operator=(const Sprite& other) noexcept
    {
        if(this != &other)
        {
            this->m_pos             = other.m_pos;
            this->m_size            = other.m_size;
            this->m_texture         = other.m_texture;
            this->m_rect            = other.m_rect;
            this->m_texSize         = other.m_texSize;
            this->m_vertexData      = other.m_vertexData;
            this->m_origin          = other.m_origin;
            this->m_scale           = other.m_scale;
            this->m_rotation        = other.m_rotation;
            m_indexData = {0,1,2, 1,3,2};
        }

        return *this;
    }

    Sprite& Sprite::operator=(Sprite&& other) noexcept
    {
        std::swap(m_pos, other.m_pos);
        std::swap(m_size, other.m_size);
        std::swap(m_rect, other.m_rect);
        std::swap(m_texSize, other.m_texSize);
        std::swap(m_vertexData, other.m_vertexData);
        std::swap(m_origin, other.m_origin);
        std::swap(m_scale, other.m_scale);
        std::swap(m_rotation, other.m_rotation);
        m_indexData = {0,1,2, 1,3,2};
        m_texture = std::move(other.m_texture);

        return *this;
    }

    bool Sprite::LoadFromFile(const String& filename) noexcept
    {
        auto newTexture = std::make_shared<Texture>();

        if(!newTexture->LoadFromFile(filename))
        {
            m_texture = nullptr;

            return false;
        }

        return LoadFromTexture(newTexture);
    }

    bool Sprite::LoadFromImage(const Image& image) noexcept
    {
        if(image.GetData() == nullptr)
        {
            m_texture = nullptr;

            return false;
        }

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
        {
            return false;
        }

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
                interpolate ? Texture::Linear : Texture::Nearest
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

    void Sprite::OnDraw(RenderTarget* renderer, Texture*) noexcept
    {
        if(m_updated)
        {
            m_vertexData.clear();

            const auto tl = m_rect.pos / m_texSize;
            const auto br = (m_rect.pos + m_rect.size) / m_texSize;
            const auto cos_r = static_cast<float>(std::cos(Math::DegreesToRadians(m_rotation)));
            const auto sin_r = static_cast<float>(std::sin(Math::DegreesToRadians(m_rotation)));
            const auto offset = m_origin * -1;

            m_vertexData.push_back(Vertex{offset,                           {tl.x, br.y}, 0x0, Vertex::TEXTURE, 0});
            m_vertexData.push_back(Vertex{offset + Vector2f(m_size.x, 0.f), {br.x, br.y}, 0x0, Vertex::TEXTURE, 0});
            m_vertexData.push_back(Vertex{offset + Vector2f(0.f, m_size.y), {tl.x, tl.y}, 0x0, Vertex::TEXTURE, 0});
            m_vertexData.push_back(Vertex{offset + m_size,                  {br.x, tl.y}, 0x0, Vertex::TEXTURE, 0});

            for(auto& v : m_vertexData)
            {
                v.pos = Math::Rotate(v.pos * m_scale, cos_r, sin_r) + m_pos;
            }

            m_updated = false;
        }

        if(m_texture)
        {
            renderer->PushVertexData(m_vertexData, m_indexData, *m_texture);
        }
    }
}