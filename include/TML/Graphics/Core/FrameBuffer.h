#pragma once
#include <TML/Types.h>
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
        ui32 m_id;
    };
}