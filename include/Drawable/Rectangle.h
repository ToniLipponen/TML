#pragma once
#include "Drawable.h"
#include "../Types.h"

namespace tml {
    class Rectangle : public Drawable {
    public:
        Rectangle() {
            m_pos = {0, 0};
            m_size = {100, 100};
            m_color = {255, 255, 255, 255};
        }

        Rectangle(float x, float y, float w, float h) {
            m_pos = {x, y};
            m_size = {w, h};
            m_color = {255, 255, 255, 255};
        }

    private:

    };
};