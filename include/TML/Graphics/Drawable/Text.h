#pragma once
#include <TML/Graphics/Font.h>
#include <TML/Graphics/Drawable/Vertex.h>
#include <TML/Graphics/Drawable/Drawable.h>
#include <TML/System/String.h>

namespace tml
{
    class TML_API Text : public Drawable
    {
    public:
        Text();
        explicit Text(String string, const Vector2f& pos = {0,0}, float size = 30, const Color& color = Color::White);

        void SetSize(float s) noexcept;
        void SetString(const String& string) noexcept;
        [[maybe_unused]] void SetFont(const Font& font) noexcept;
        [[maybe_unused]] void SetFont(const std::shared_ptr<Font>& font) noexcept;
        [[maybe_unused]] void SetLeading(float spacing) noexcept;   //!< Leading is the vertical spacing between lines of text.
        [[maybe_unused]] void SetTracking(float tracking) noexcept; //!< Tracking is the horizontal spacing between characters.
        [[maybe_unused]] void SetKerning(float kerning) noexcept;   //!< Kerning is the horizontal spacing between different characters defined by the font.
        [[nodiscard]] const String& GetString() const noexcept;
        Vector2f GetDimensions() noexcept;
        Vector2f GetCenter() noexcept override;
        Vector2f GetOffsetToIndex(uint64_t index) noexcept;

    private:
        void Generate();
        void OnDraw(class RenderTarget* renderer, class Texture*) noexcept override;
        static Font& GetDefaultFont() noexcept;

    private:
        float m_leading  = 0;
        float m_tracking = 0;
        float m_kerning  = 0;
        Vector2f m_dimensions;
        std::shared_ptr<Font> m_font = nullptr;
        String m_string;

    private:
        using Transformable::SetSize;
        using Transformable::GetSize;
    };
}