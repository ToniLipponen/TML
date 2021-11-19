#pragma once
#include "Drawable.h"
#include "TML/Graphics/Image.h"
#include "TML/Utilities/Condition.h"

namespace tml
{
    // Rectangle for selecting a part of a texture to draw
    struct TexRect
    {
        Vector2f pos;
        Vector2f size;
    };

    class Sprite : public Drawable {
    public:
        Sprite()
        : m_rect({{0, 0}, {0, 0}}), m_texSize(0)
        {
            m_pos = Vector2f{0,0};
            m_size = Vector2f{0,0};
        }

        void SetRect(const TexRect &r) {
            m_rect = r;
        }

        void LoadFromFile(const std::string& filename)
        {
            m_img.LoadFromFile(filename);
            m_size = Vector2f(m_img.GetWidth(), m_img.GetHeight());
            m_texSize = m_size;
            m_rect = {{0,0}, m_size};
            m_tex.LoadFromMemory(m_img.GetWidth(), m_img.GetHeight(), m_img.GetBpp(), m_img.GetData());
        }

        void LoadFromImage(const Image& image)
        {
            m_img = image;
            m_size = Vector2f(m_img.GetWidth(), m_img.GetHeight());
            m_texSize = m_size;
            m_rect = {{0,0}, m_size};
            m_tex.LoadFromMemory(m_img.GetWidth(), m_img.GetHeight(), m_img.GetBpp(), m_img.GetData());
        }

        void SetInterpolation(bool interpolate)
        {
            m_tex.SetMinMagFilter(
                    tml::Condition(interpolate, Texture::Linear, Texture::Nearest),
                    tml::Condition(interpolate, Texture::Linear, Texture::Nearest)
            );
        }

        friend class Renderer;

    private:
        Image m_img;
        TexRect m_rect;
        Vector2f m_texSize;
    };
};