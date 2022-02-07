#pragma once
#include "TML/Utilities/Types.h"
#include "TML/Graphics/Vector2.h"
#include "TML/Graphics/Image.h"
#include <string>

namespace tml
{
    class Texture
    {
    public:
        enum ClampMode
        {
            ClampToBorder       = 0x812D,
            ClampToEdge         = 0x812F,
            MirrorClampToEdge   = 0x8743,
            Repeat              = 0x2901,
            MirrorRepeat        = 0x8370,
        };
        enum Filter
        {
            Nearest                 = 0x2600,
            Linear                  = 0x2601,
            NearestMipmapNearest    = 0x2700,
            LinearMipmapNearest     = 0x2701,
            NearestMipmapLinear     = 0x2702,
            LinearMipmapLinear      = 0x2703,
        };
    public:
        Texture();
        ~Texture();
        void LoadFromImage(Image& image);
        void LoadFromFile(const std::string& filename);
        void LoadFromMemory(i32 w, i32 h, ui8 bpp, ui8* data);

        void Bind(ui32 slot = 0) const;
        void SetMinMagFilter(Filter min, Filter mag);
        void SetClampMode(ClampMode mode);

        inline constexpr Filter     GetMinFilter() const noexcept { return m_minFilter; }
        inline constexpr Filter     GetMagFilter() const noexcept { return m_magFilter; }
        inline constexpr ClampMode  GetClampMode() const noexcept { return m_clampMode; }
        inline constexpr ui32       GetID()        const noexcept { return m_id;        }

    private:
        inline void Generate() const;
        
    private:
        ClampMode m_clampMode = ClampToBorder;
        Filter m_minFilter = LinearMipmapLinear;
        Filter m_magFilter = LinearMipmapLinear;
        ui32 m_id;
        i32 m_width, m_height, m_bpp;
        ui8 *m_pixelData = nullptr;
    };
}