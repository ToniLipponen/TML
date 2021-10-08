#pragma once
#include "../Font.h"
#include "Vertex.h"
#include "Drawable.h"
#include <vector>
#include <string>

namespace tml
{
    class Text : public Drawable {
    public:
        // Initializes a Text with the default font and no string content.
        Text();
        // Initializes a Text with the default font
        explicit Text(std::wstring text);
        Text(std::wstring text, const std::string &font_file_name);
        Text(std::wstring text, Font &font);
        void SetPosition(const Vector2 &pos) noexcept override;
        void SetSize(float s);
        void SetColor(const Color &color) noexcept override;
        void SetString(std::wstring string);
        void SetFont(const Font &font);
        void SetSpacing(ui32 s);
        inline Vector2 GetDimensions() const noexcept { return m_dimensions; }
        friend class Renderer;

    private:
        void Generate() noexcept;
        Font m_font;
        std::wstring m_string;
        std::vector<Vertex> m_vertexData;
        std::vector<ui32> m_indexData;
        float m_lineSpacing = 0;
        float m_kerning = 0;
        Vector2 m_dimensions;
    };
}