#pragma once
#include <string>
#include <map>
#include <unordered_map>

#include "TML/Types.h"
#include "TML/System/Math/Vector2.h"
#include "TML/Graphics/Core/Texture.h"

namespace tml
{
    class Font
    {
    public:
        Font();
        Font(const Font& rhs);
        ~Font();
        Font& operator=(const Font& rhs);

        void LoadFromFile(const std::string &filename);
        void LoadFromMemory(const ui8 *data, ui32 size);
        friend class Text;
        friend class Renderer;

    private:
        void* m_cdata;
        Texture m_texture;
        ui8* m_bitmap = nullptr;
    };
}