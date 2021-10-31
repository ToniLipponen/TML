#pragma once
#include <string>
#include <map>
#include <unordered_map>

#include "Types.h"
#include "Vector2.h"
#include "Texture.h"

namespace tml
{
    class Font
    {
    public:
        Font();
        ~Font();
        void LoadFromFile(const std::string &filename);
        void LoadFromMemory(const ui8 *data, ui32 size);
        friend class Text;
        friend class Renderer;
    private:
        void* m_cdata;
        void* m_font_info;
        Texture m_texture;
    };
}