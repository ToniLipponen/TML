#pragma once
#include <string>
#include <map>
#include <unordered_map>

#include <TML/System/Math/Vector2.h>
#include <TML/Graphics/Core/Texture.h>

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
        void LoadFromMemory(const uint8_t *data);
        friend class Text;
        friend class Renderer;

    protected:
        void GetAlignedQuad(void* output, int codePoint, float& x, float& y);

    private:
        void* m_cdata;
        Texture m_texture;
    };
}