#pragma once
#include <TML/Graphics/Core/Texture.h>
#include <TML/Graphics/RenderTarget.h>

namespace tml
{
    class RenderTexture : public RenderTarget
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
        Vector2f GetRenderTargetSize() const noexcept override { return {m_width, m_height}; }
        i32 m_width = 0;
        i32 m_height = 0;
        Texture m_texture;
        class FrameBuffer* m_frameBuffer;
    };
}