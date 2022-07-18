#pragma once
#include <TML/Interface/Components/Button.h>
#include <TML/Graphics/Drawable/Sprite.h>

namespace tml::Interface
{
    class TML_API ImageButton : public Component
    {
    public:
        explicit ImageButton(
                const Image& image,
                uint32_t height = 20,
                int32_t x = 0,
                int32_t y = 0) noexcept;

        void SetImage(const Image& image, uint32_t height = 20) noexcept;

    private:
        virtual void pDraw(RenderTarget& renderTarget) noexcept override;

    protected:
        Sprite m_texture;
    };
}