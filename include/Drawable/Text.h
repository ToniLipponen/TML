#pragma once
#include "../Font.h"
#include "Vertex.h"
#include "Drawable.h"
#include <vector>
#include <string>

namespace tml {
    class Text : public Drawable {
    public:
        // Initializes a Text with the default font and no string content.
        Text();

        // Initializes a Text with the default font
        Text(const std::string &text);

        Text(const std::string &text, const std::string &font_file_name);

        Text(const std::string &text, Font &font);

        void SetPosition(const Vector2 &pos) override;

        void SetSize(const Vector2 &size) override;

        void SetColor(const Color &color) override;

        void SetString(const std::string &string);

        void SetFont(const Font &font);

        void SetSpacing(ui32 s);

        friend class Renderer;

    private:
        void Generate();

        Font m_font;
        std::string m_string;
        std::vector<Vertex> m_vertexData;
        std::vector<ui32> m_indexData;
        ui32 m_spacing = 1;

    };
};