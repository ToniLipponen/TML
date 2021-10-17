#pragma once
#include "Drawable.h"
#include "../Image.h"
#include "../Utilities/Condition.h"
#include <chrono>
#include <thread>

namespace tml {
// Rectangle for selecting a part of a texture to draw
    struct TexRect {
        Vector2 pos;
        Vector2 size;
    };

    class Sprite : public Drawable {
    public:
        Sprite() {
            m_rect = {{0, 0},
                      {0, 0}};
        }

        void SetRect(const TexRect &r) {
            m_rect = r;
        }

        void LoadFromFile(const std::string& filename)
        {
            m_img.LoadFromFile(filename);
            m_size = Vector2(m_img.GetWidth(), m_img.GetHeight());
            m_texSize = m_size;
            m_rect = {{0,0}, m_size};
            m_tex.LoadFromMemory(m_img.GetWidth(), m_img.GetHeight(), m_img.GetBpp(), m_img.GetData());
            std::this_thread::sleep_for(std::chrono::microseconds(500));
        }

        void LoadFromImage(const Image& image)
        {
            m_img = image;
            m_size = Vector2(m_img.GetWidth(), m_img.GetHeight());
            m_texSize = m_size;
            m_rect = {{0,0}, m_size};
            m_tex.LoadFromMemory(m_img.GetWidth(), m_img.GetHeight(), m_img.GetBpp(), m_img.GetData());
            std::this_thread::sleep_for(std::chrono::microseconds(500));
        }

        void SetInterpolation(bool interpolate)
        {
            m_tex.SetMinMagFilter(
                    tml::Condition(interpolate, Texture::Linear, Texture::Nearest),
                    tml::Condition(interpolate, Texture::Linear, Texture::Nearest));
        }

        friend class Renderer;

    private:
        Image m_img;
        TexRect m_rect;
        Vector2 m_texSize;
    };
};