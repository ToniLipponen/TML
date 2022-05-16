#pragma once
#include <TML/System/Image.h>
#include <TML/Graphics/Core/Texture.h>
#include <TML/Graphics/Drawable/Drawable.h>

namespace tml
{
    /// Rectangle for selecting a part of a texture to draw
    /// Note: Pos and size are in pixel coordinates, not UV.
    struct TexRect
    {
        Vector2f pos;
        Vector2f size;
    };

    class TML_API Sprite : public Drawable
    {
    public:
        Sprite();
        bool LoadFromFile(const String& filename) noexcept;
        bool LoadFromImage(const Image& image) noexcept;
        bool LoadFromTexture(const Texture& texture) noexcept;
        bool LoadFromTexture(const std::shared_ptr<Texture>& texture) noexcept;
        void SetInterpolation(bool interpolate) noexcept;
        void SetRect(const TexRect& r) noexcept;
        TexRect GetRect() const noexcept;
        Vector2f GetTextureSize() const noexcept;
        const std::shared_ptr<Texture>& GetTexture() const noexcept;

    protected:
        void OnDraw(class RenderTarget*, class Texture*) noexcept override;

    private:
        TexRect m_rect;
        Vector2f m_texSize;
        std::shared_ptr<Texture> m_texture = nullptr;
    };
};