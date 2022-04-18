#pragma once
#include <TML/Graphics/Core/Texture.h>

namespace tml
{
    class FrameBuffer
    {
    public:
        FrameBuffer();
        ~FrameBuffer();
        void Bind() const noexcept;
        void Unbind() const noexcept;
        bool AttachTexture(const Texture& texture) const;

    private:
        uint32_t m_id;
    };
}