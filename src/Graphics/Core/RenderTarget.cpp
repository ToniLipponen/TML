#include <TML/Graphics/Core/RenderTarget.h>
#include "../../Headers/GLHeader.h"

namespace tml
{
    RenderTarget::RenderTarget()
    : m_id(0)
    {
        GL_CALL(glad_glGenFramebuffers(1, &m_id));
    }

    RenderTarget::~RenderTarget()
    {
        GL_CALL(glad_glDeleteFramebuffers(1, &m_id));
    }

    void RenderTarget::Bind() const noexcept
    {
        GL_CALL(glad_glBindFramebuffer(GL_FRAMEBUFFER, m_id));
    }

    void RenderTarget::Unbind() const noexcept
    {
        GL_CALL(glad_glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    bool RenderTarget::AttachTexture(const Texture &texture) const
    {
        Bind();
        GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
#ifdef TML_USE_GLES
        GL_CALL(glad_glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.GetID(), 0));
#else
        GL_CALL(glad_glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture.GetID(), 0));
#endif
        GL_CALL(glad_glDrawBuffers(1, drawBuffers));
        return (glad_glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    }
}