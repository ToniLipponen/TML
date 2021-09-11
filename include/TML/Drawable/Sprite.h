#pragma once
#include "Drawable.h"


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

        Sprite(const std::string &filename) {
            m_tex.LoadFromFile(filename.c_str());
            m_pos = {0, 0};
            m_size = m_tex.GetSize();
            m_rect = {m_pos, m_size};
        }

        Sprite(Texture &texture) {
            m_tex = texture;
            m_pos = {0, 0};
            m_size = m_tex.GetSize();
            m_rect = {m_pos, m_size};
        }

        void SetRect(const TexRect &r) {
            m_rect = r;
        }

        friend class Renderer;

    private:
        TexRect m_rect;
    };
};