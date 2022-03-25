#pragma once
#include <TML/Graphics/Font.h>
#include <TML/Graphics/Drawable/Vertex.h>
#include <TML/Graphics/Drawable/Drawable.h>
#include <TML/System/String.h>

namespace tml
{
    class Text : public Drawable
    {
    public:
        Text();
        void SetSize(float s) noexcept;
        void SetString(const String& string) noexcept;
        void SetFont(const Font& font) noexcept;
        void SetFont(const std::shared_ptr<Font>& font) noexcept;
        void SetSpacing(float spacing) noexcept;
        void SetKerning(float kerning) noexcept;

        inline constexpr const String& GetString() const noexcept { return m_string; }
        Vector2f GetDimensions() noexcept;
        
        void Rotate(float)                       = delete;
        void SetRotation(float)                  = delete;
        const Vector2f& GetSize(const Vector2f&) = delete;
        constexpr float GetRotation()            = delete;

    private:
        void Generate();
        void OnDraw(class Renderer*, Texture*) noexcept override;

    private:
        std::shared_ptr<Font> m_font = nullptr;
        String m_string;
        float m_lineSpacing = 0;
        float m_kerning = 0;
        Vector2f m_dimensions;
        static Font* s_defaultFont;
    };
}