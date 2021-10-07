#pragma once
#include "Drawable.h"
#include "../Image.h"

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
            Image img(filename);
            m_size = Vector2(img.GetWidth(), img.GetHeight());
            m_texSize = m_size;
            m_tex.LoadFromMemory(img.GetWidth(), img.GetHeight(), img.GetBpp(), img.GetData());
        }

        friend class Renderer;

    private:
        TexRect m_rect;
        Vector2 m_texSize;
    };
};