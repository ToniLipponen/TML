#pragma once
#include <TML/Export.h>
#include <TML/System/Math/Vector2.h>
#include <TML/System/Image.h>

namespace tml
{
    class TML_API Texture
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
        Texture() noexcept;
        Texture(const Texture&) = delete;
        Texture(Texture&& rhs) noexcept;
        ~Texture() noexcept;

        Texture& operator=(const Texture& rhs) noexcept;
        Texture& operator=(Texture&& rhs) noexcept;
        bool LoadFromImage(const Image& image) noexcept;
        bool LoadFromFile(const String& filename) noexcept;
        bool LoadFromMemory(int32_t w, int32_t h, uint8_t bpp, const uint8_t* data) noexcept;

        void Bind(uint32_t slot = 0) const noexcept;
        void BindToImageSlot(uint32_t slot = 0) const noexcept;
        void SetMinMagFilter(Filter min, Filter mag) noexcept;
        void SetClampMode(ClampMode mode) noexcept;

        inline constexpr Filter    GetMinFilter() const noexcept { return m_minFilter; }
        inline constexpr Filter    GetMagFilter() const noexcept { return m_magFilter; }
        inline constexpr ClampMode GetClampMode() const noexcept { return m_clampMode; }
        inline constexpr uint32_t  GetID()        const noexcept { return m_id;        }
        inline constexpr uint32_t  GetWidth()     const noexcept { return m_width;     }
        inline constexpr uint32_t  GetHeight()    const noexcept { return m_height;    }
        inline constexpr uint32_t  GetBpp()       const noexcept { return m_bpp;       }
        Image                      GetData()      const noexcept;
        
    protected:
        inline void Update() const noexcept;
        inline void Upload(const void* data) const noexcept;
        ClampMode m_clampMode = ClampToEdge;
        Filter m_minFilter = LinearMipmapLinear;
        Filter m_magFilter = Linear;

        uint32_t m_id;
        int32_t m_width, m_height, m_bpp, m_internalFormat, m_format;
    };
}