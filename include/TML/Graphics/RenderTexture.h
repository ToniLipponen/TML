#pragma once
#include <TML/Graphics/Core/Texture.h>
#include <TML/Graphics/RenderTarget.h>
#include <TML/Export.h>

namespace tml
{
    class TML_API RenderTexture : public RenderTarget
    {
    public:
        RenderTexture();
        virtual ~RenderTexture() noexcept;
        void Create(int32_t width, int32_t height) noexcept;
        bool SetActive(bool active = true);
        void Clear() noexcept override;
        void Display() noexcept;
        const Texture& GetTexture() const noexcept;
        
    private:
        Vector2f GetRenderTargetSize() const noexcept override { return {m_width, m_height}; }
        int32_t m_width = 0;
        int32_t m_height = 0;
        Texture m_texture;
        class FrameBuffer* m_frameBuffer;
    };
}