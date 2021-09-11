#pragma once
#include "Types.h"
#include "Vector2.h"

#ifndef UINT_MAX
    #define UINT_MAX 0xffffffff
#endif

namespace tml {
    class Texture {
    public:
        enum [[maybe_unused]] ClampMode {
            ClampToBorder = 0x812D,
            ClampToEdge = 0x812F,
            MirrorClampToEdge = 0x8743,
            Repeat = 0x2901,
            MirrorRepeat = 0x8370,
        };
        enum [[maybe_unused]] Filter {
            Nearest = 0x2600,
            Linear = 0x2601,
            NearestMipmapNearest = 0x2700,
            LinearMipmapNearest = 0x2701,
            NearestMipmapLinear = 0x2702,
            LinearMipmapLinear = 0x2703,
        };
    public:
        Texture();
        [[maybe_unused]] explicit Texture(cstring filename);
        [[maybe_unused]] Texture(i32 w, i32 h, ui8 bpp, ui8 *data);
        ~Texture();

        void LoadFromFile(cstring filename);
        void LoadFromMemory(i32 w, i32 h, ui8 bpp, ui8* data); // @data has to be alive throughout the use of this texture
        [[maybe_unused]] void CopyFromMemory(i32 w, i32 h, ui8 Bpp, ui8* data); // Makes a copy of @data
        void Bind(ui32 slot = 0) const;
        [[maybe_unused]] void SetMipMapLevel(ui8 level);
        void SetMinMagFilter(Filter min, Filter mag);
        void SetClampMode(ClampMode mode);

        constexpr bool HasData() const noexcept {
            return m_id != UINT_MAX;
        }

        constexpr ui32 GetID() const {
            return m_id;
        }

        Vector2 GetSize() const noexcept {
            return Vector2{static_cast<float>(m_width), static_cast<float>(m_height)};
        }

        [[maybe_unused]] constexpr ui32 GetWidth() const noexcept {
            return m_width;
        }

        [[maybe_unused]] constexpr ui32 GetHeight() const noexcept {
            return m_height;
        }

    private:
        void Generate();

    private:
        ClampMode m_clampmode = Repeat;
        Filter m_minfilter = LinearMipmapLinear;
        Filter m_magfilter = Linear;
        ui32 m_id = UINT_MAX;
        i32 m_width, m_height, m_bpp;
        ui8 *m_pixeldata = nullptr;
        ui8 m_mipmap_level = 0;
        bool m_allocated = false;
    };
}