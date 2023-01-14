#include <TML/Graphics/Core/FrameBuffer.h>
#include "../Headers/GLHeader.h"

namespace tml
{
    FrameBuffer::FrameBuffer()
    : m_id(0)
    {
#if defined(TML_USE_GLES) || !defined(TML_USE_DSA)
        GL_CALL(glad_glGenFramebuffers(1, &m_id));
#else
        GL_CALL(glad_glCreateFramebuffers(1, &m_id));
#endif
    }

    FrameBuffer::~FrameBuffer()
    {
        GL_CALL(glad_glDeleteFramebuffers(1, &m_id));
    }

    void FrameBuffer::Bind() const noexcept
    {
        GL_CALL(glad_glBindFramebuffer(GL_FRAMEBUFFER, m_id));
    }

    void FrameBuffer::Unbind() const noexcept
    {
        GL_CALL(glad_glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    bool FrameBuffer::AttachTexture(const Texture &texture) const noexcept
    {
#if defined(TML_USE_GLES) || !defined(TML_USE_DSA)
        Bind();
        GL_CALL(glad_glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.GetID(), 0));

        return (glad_glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
#else
        GL_CALL(glad_glNamedFramebufferTexture(m_id, GL_COLOR_ATTACHMENT0, texture.GetID(), 0));

        return (glad_glCheckNamedFramebufferStatus(m_id, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
#endif
    }
}