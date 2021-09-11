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
        explicit Text(std::string text);
        Text(std::string text, const std::string &font_file_name);
        Text(std::string text, Font &font);
        void SetPosition(const Vector2 &pos) noexcept override;
        void SetSize(float s);
        void SetColor(const Color &color) noexcept override;
        [[maybe_unused]] void SetString(std::string string);
        void SetFont(const Font &font);
        [[maybe_unused]] void SetSpacing(ui32 s);
        [[maybe_unused]] inline Vector2 GetSize() const noexcept { return {m_width, m_height}; }
        friend class Renderer;

    private:
        void Generate() noexcept;
        Font m_font;
        std::string m_string;
        std::vector<Vertex> m_vertexData;
        std::vector<ui32> m_indexData;
        ui32 m_spacing = 1;
        float m_width = 0, m_height = 0;
    };
}