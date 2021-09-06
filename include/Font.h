#pragma once
#include <string>
#include <map>
#include <unordered_map>

#include "Types.h"
#include "Vector2.h"
#include "Texture.h"

namespace tml {
    class Font {
    public:
        Font();
        Font(const std::string &filename);
        Font(const ui8 *data, ui32 datasize); // From raw file data.
        void LoadFromFile(const std::string &filename);
        void LoadFromMemory(const ui8 *data, ui32 size);
        friend class Text;
        friend class Renderer;
    private:
        struct FontChar {
            Vector2 size;
            Vector2 pos;
            Vector2 offset;
            float advance;
        };
        std::unordered_map<char, FontChar> m_chars;
        Texture m_texture;
    };
};