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
        void SetSize(float s);
        void SetString(const String& string, const String& font = "");
        void SetFont(const Font &font);
        void SetSpacing(float spacing);
        void SetKerning(float kerning);

        inline constexpr const String& GetString() const noexcept { return m_string; }
        Vector2f GetDimensions() noexcept;
        friend class Renderer;

    private:
        void SetTexture()               = delete;
        void Rotate()                   = delete;
        void SetRotation()              = delete;
        const Vector2f& GetSize()       = delete;
        constexpr float GetRotation()   = delete;

    private:
        void Generate();
        void OnDraw(class Renderer*, Texture*) noexcept override;
        Font m_font;
        bool m_hasFont = false;
        String m_string;
        float m_lineSpacing = 0;
        float m_kerning = 0;
        Vector2f m_dimensions;
        static Font* s_defaultFont;
    };
}