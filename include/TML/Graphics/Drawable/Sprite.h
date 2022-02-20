#pragma once
#include <TML/System/Image.h>
#include <TML/Graphics/Core/Texture.h>
#include <TML/Graphics/Drawable/Drawable.h>

namespace tml
{
    // Rectangle for selecting a part of a texture to draw
    struct TexRect
    {
        Vector2f pos;
        Vector2f size;
    };

    class Sprite : public Drawable
    {
    public:
        Sprite();
        void SetRect(const TexRect &r);
        void LoadFromFile(const String& filename);
        void LoadFromImage(const Image& image);
        void SetInterpolation(bool interpolate);
        friend class Renderer;

    protected:
        void Generate() noexcept override;

    private:
        Image m_img;
        TexRect m_rect;
        Texture m_tex;
        Vector2f m_texSize;
    };
};