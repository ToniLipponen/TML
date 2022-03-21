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
        bool LoadFromFile(const String& filename) noexcept;
        bool LoadFromImage(const Image& image) noexcept;
        void SetInterpolation(bool interpolate) noexcept;
        void SetSharedTexture(const std::shared_ptr<Texture>& texture) noexcept;
        void SetRect(const TexRect& r) noexcept;
        TexRect GetRect() const noexcept;
        Vector2f GetTextureSize() const noexcept;

    protected:
        void OnDraw(class Renderer*, Texture*) noexcept override;

    private:
        Image m_img;
        TexRect m_rect;
        Texture m_tex;
        Vector2f m_texSize;
        std::shared_ptr<Texture> m_sharedResource = nullptr;
    };
};