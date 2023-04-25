#pragma once
#include <cstdint>
#include <TML/System/Math.h>
#include <TML/Export.h>

namespace tml
{
    struct Color
    {
        constexpr Color() noexcept = default;

        constexpr Color(uint8_t r, uint8_t g, uint8_t b) noexcept
        : r(r), g(g), b(b), a(255)
        {

        }

        constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept
        : r(r), g(g), b(b), a(a)
        {

        }

        explicit constexpr Color(uint32_t hex) noexcept
        : r((hex & 0xff000000u) >> 24),
          g((hex & 0x00ff0000u) >> 16),
          b((hex & 0x0000ff00u) >> 8),
          a((hex & 0x000000ffu))
        {

        }

		Color& operator=(uint32_t hex) noexcept
		{
			r = (hex & 0xff000000u) >> 24;
			g = (hex & 0x00ff0000u) >> 16;
			b = (hex & 0x0000ff00u) >>  8;
			a =  hex & 0x000000ffu;

			return *this;
		}

        inline constexpr Color& operator+=(const Color& color) noexcept;
        inline constexpr Color& operator-=(const Color& color) noexcept;
        [[nodiscard]] inline constexpr Color operator+(const Color& color) const noexcept;
        [[nodiscard]] inline constexpr Color operator-(const Color& color) const noexcept;
        [[nodiscard]] inline constexpr bool operator==(const Color& color) const noexcept;
        [[nodiscard]] inline constexpr bool operator!=(const Color& color) const noexcept;

        [[nodiscard]] inline constexpr uint32_t Hex() const noexcept
        {
            return  (static_cast<uint32_t>(r) << 24) |
                    (static_cast<uint32_t>(g) << 16) |
                    (static_cast<uint32_t>(b) << 8)  |
                    static_cast<uint32_t>(a);
        }

        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
        uint8_t a = 0;

        [[maybe_unused]] static const Color Red;
        [[maybe_unused]] static const Color Green;
        [[maybe_unused]] static const Color Blue;
        [[maybe_unused]] static const Color Yellow;
        [[maybe_unused]] static const Color Magenta;
        [[maybe_unused]] static const Color Cyan;
        [[maybe_unused]] static const Color Black;
        [[maybe_unused]] static const Color White;
        [[maybe_unused]] static const Color Transparent;
    };

    inline constexpr Color& Color::operator+=(const Color& color) noexcept
    {
        r = static_cast<uint8_t>(Math::Min<int32_t>(static_cast<int32_t>(r) + color.r, 255));
        g = static_cast<uint8_t>(Math::Min<int32_t>(static_cast<int32_t>(g) + color.g, 255));
        b = static_cast<uint8_t>(Math::Min<int32_t>(static_cast<int32_t>(b) + color.b, 255));
        a = static_cast<uint8_t>(Math::Min<int32_t>(static_cast<int32_t>(a) + color.a, 255));
        return *this;
    }

    inline constexpr Color& Color::operator-=(const Color& color) noexcept
    {
        r = static_cast<uint8_t>(Math::Max<int32_t>(static_cast<int32_t>(r) - color.r, 0));
        g = static_cast<uint8_t>(Math::Max<int32_t>(static_cast<int32_t>(g) - color.g, 0));
        b = static_cast<uint8_t>(Math::Max<int32_t>(static_cast<int32_t>(b) - color.b, 0));
        a = static_cast<uint8_t>(Math::Max<int32_t>(static_cast<int32_t>(a) - color.a, 0));
        return *this;
    }

    [[nodiscard]] inline constexpr Color Color::operator+(const Color& color) const noexcept
    {
        return
        {
            static_cast<uint8_t>(Math::Min<int32_t>(static_cast<int32_t>(r) + color.r, 255)),
            static_cast<uint8_t>(Math::Min<int32_t>(static_cast<int32_t>(g) + color.g, 255)),
            static_cast<uint8_t>(Math::Min<int32_t>(static_cast<int32_t>(b) + color.b, 255)),
            static_cast<uint8_t>(Math::Min<int32_t>(static_cast<int32_t>(a) + color.a, 255))
        };
    }

    [[nodiscard]] inline constexpr Color Color::operator-(const Color& color) const noexcept
    {
        return
        {
            static_cast<uint8_t>(Math::Max<int32_t>(static_cast<int32_t>(r) - color.r, 0)),
            static_cast<uint8_t>(Math::Max<int32_t>(static_cast<int32_t>(g) - color.g, 0)),
            static_cast<uint8_t>(Math::Max<int32_t>(static_cast<int32_t>(b) - color.b, 0)),
            static_cast<uint8_t>(Math::Max<int32_t>(static_cast<int32_t>(a) - color.a, 0))
        };
    }

    [[nodiscard]] inline constexpr bool Color::operator==(const Color& color) const noexcept
    {
        return Hex() == color.Hex();
    }

    [[nodiscard]] inline constexpr bool Color::operator!=(const Color& color) const noexcept
    {
        return Hex() != color.Hex();
    }

    template<>
    inline constexpr Color Math::Lerp<Color>(const Color& a, const Color& b, double m) noexcept
    {
        return
        {
            Lerp(a.r, b.r, m),
            Lerp(a.g, b.g, m),
            Lerp(a.b, b.b, m),
            Lerp(a.a, b.a, m)
        };
    }

    inline constexpr Color Color::Red         = Color(0xff0000ff);
    inline constexpr Color Color::Green       = Color(0x00ff00ff);
    inline constexpr Color Color::Blue        = Color(0x0000ffff);
    inline constexpr Color Color::Yellow      = Color(0xffff00ff);
    inline constexpr Color Color::Magenta     = Color(0xff00ffff);
    inline constexpr Color Color::Cyan        = Color(0x00ffffff);
    inline constexpr Color Color::Black       = Color(0x000000ff);
    inline constexpr Color Color::White       = Color(0xffffffff);
    inline constexpr Color Color::Transparent = Color(0x00000000);
}