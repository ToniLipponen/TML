#pragma once
#include "Types.h"
#include "Vector2.h"
#include "Image.h"
#include <string>

namespace tml {
    class Texture {
    public:
        enum ClampMode {
            ClampToBorder = 0x812D,
            ClampToEdge = 0x812F,
            MirrorClampToEdge = 0x8743,
            Repeat = 0x2901,
            MirrorRepeat = 0x8370,
        };
        enum Filter {
            Nearest = 0x2600,
            Linear = 0x2601,
            NearestMipmapNearest = 0x2700,
            LinearMipmapNearest = 0x2701,
            NearestMipmapLinear = 0x2702,
            LinearMipmapLinear = 0x2703,
        };
    public:
        Texture();
        ~Texture();
        void LoadFromImage(Image& image);
        void LoadFromFile(const std::string& filename);
        void LoadFromMemory(i32 w, i32 h, ui8 bpp, ui8* data); // @data has to be alive throughout the use of this texture
        void Bind(ui32 slot = 0) const;
        void SetMipMapLevel(ui8 level);
        void SetMinMagFilter(Filter min, Filter mag);
        void SetClampMode(ClampMode mode);
    friend class Renderer;
    private:
        void Generate();
        constexpr ui32 GetID() const {
            return m_id;
        }
    private:
        ClampMode m_clampmode = ClampToBorder;
        Filter m_minfilter = LinearMipmapNearest;
        Filter m_magfilter = Linear;
        ui32 m_id;
        i32 m_width, m_height, m_bpp;
        ui8 *m_pixeldata = nullptr;
        ui8 m_mipmap_level = 0;
    };
}