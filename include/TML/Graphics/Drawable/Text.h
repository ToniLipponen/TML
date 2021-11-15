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
        explicit Text(const std::string& text);
        Text(const std::string& text, const std::string &fontFileName);
        Text(const std::string& text, Font &font);
        void SetPosition(const Vector2 &pos) noexcept override;
        void SetSize(float s);
        void SetColor(const Color &color) noexcept override;
        void SetString(const std::string& string);
        void SetString(const std::wstring& string);
        void SetFont(const Font &font);
        void SetSpacing(ui32 s);
        inline Vector2 GetDimensions() const noexcept { return m_dimensions; }
        friend class Renderer;

    private:
        void SetTexture() = delete;
        void Rotate() = delete;
        void SetRotation() = delete;
        const Vector2& GetSize() = delete;
        constexpr float GetRotation() = delete;

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