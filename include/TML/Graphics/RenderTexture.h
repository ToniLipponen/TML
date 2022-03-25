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

        void Create(i32 width, i32 height) noexcept;
        bool SetActive(bool active = true);
        void Clear() noexcept override;
        void Display() noexcept;
        const Texture& GetTexture() const noexcept;
        
    private:
        i32 m_width = 0;
        i32 m_height = 0;
        Texture m_texture;
        class RenderTarget* m_target;
    };
}