#pragma once
#include <TML/System/Math/Vector2.h>
#include <TML/Graphics/Core/Texture.h>
#include <TML/Export.h>
#include <unordered_map>
#include <map>

namespace std
{
    template<>
    struct hash<std::pair<int, int>>
    {
        std::size_t operator()(const std::pair<int, int>& p) const
        {
            std::hash<int> _hash;
            return _hash(p.first) ^ _hash(p.second);
        }
    };
}

namespace tml
{
    class TML_API Font
    {
    public:
        Font();
        Font(const Font& rhs);
        ~Font();

        Font& operator=(const Font& rhs);
        Font& operator=(Font&& rhs) noexcept;

        bool LoadFromFile(const String& filename) noexcept;
        bool LoadFromData(const uint8_t *data) noexcept; //!< Load a font from raw file data.

        friend class Text;
        friend class Renderer;

    private:
        using CodePoint = int;
        using Kerning   = int;

    protected:
        void GetAlignedQuad(void* output, int codePoint, float& x, float& y);
        float GetKerning(const std::pair<CodePoint, CodePoint>& pair) const noexcept;
        float GetKerning(CodePoint a, CodePoint b) const noexcept;

    private:
        bool MakeKerningTable(const uint8_t* data, int offset = 0) noexcept;
    private:
        void* m_cdata;
        Texture m_texture;
        std::unordered_map<std::pair<CodePoint, CodePoint>, Kerning> m_kerningMap;
    };
}
