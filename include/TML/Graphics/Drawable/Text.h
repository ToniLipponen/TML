#pragma once
#include "TML/Graphics/Font.h"
#include "Vertex.h"
#include "Drawable.h"
#include <vector>
#include <string>

namespace tml
{
    class Text : public Drawable
    {
    public:
        Text();
        void SetSize(float s);
        void SetString(const std::string& string, const std::string& font = "");
        void SetString(const std::wstring& string, const std::string& font = "");
        void SetFont(const Font &font);
        void SetSpacing(ui32 s);
        inline Vector2f GetDimensions() const noexcept { return m_dimensions; }
        friend class Renderer;

    private:
        void SetTexture()               = delete;
        void Rotate()                   = delete;
        void SetRotation()              = delete;
        const Vector2f& GetSize()       = delete;
        constexpr float GetRotation()   = delete;

    private:
        void Generate() noexcept override;
        Font m_font;
        std::wstring m_string;
        float m_lineSpacing = 0;
        float m_kerning = 0;
        Vector2f m_dimensions;
    };
}