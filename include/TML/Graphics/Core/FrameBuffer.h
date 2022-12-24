#pragma once
#include <TML/Graphics/Core/Texture.h>
#include <TML/Export.h>

namespace tml
{
    class TML_API FrameBuffer
    {
    public:
        FrameBuffer();
        ~FrameBuffer();
        void Bind() const noexcept;
        void Unbind() const noexcept;
        bool AttachTexture(const Texture& texture) const noexcept;

    private:
        uint32_t m_id;
    };
}