#pragma once
#include <TML/Graphics/Core/Texture.h>
#include <TML/Graphics/Renderer.h>

namespace tml
{
    class RenderTexture : public Renderer
    {
    public:
        RenderTexture();
        virtual ~RenderTexture() noexcept;

        bool Create(ui32 width, ui32 height) noexcept;
        bool SetActive(bool active = true);
        void Display() noexcept;

        const Texture& GetTexture() const noexcept;
    private:
        Texture m_texture;
        class RenderTarget* m_target;
    };
}