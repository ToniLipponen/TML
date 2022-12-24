#include <TML/Graphics/RenderTexture.h>
#include <TML/Graphics/Core/FrameBuffer.h>
#include <glad/include/glad/gl.h>

/**
 * TODO: Add depth texture
 */
namespace tml
{
    RenderTexture::RenderTexture()
    : m_frameBuffer(nullptr)
    {

    }

    void RenderTexture::Create(int32_t width, int32_t height) noexcept
    {
        if(!m_frameBuffer)
        {
            m_frameBuffer = std::make_unique<FrameBuffer>();
        }

        m_width = width;
        m_height = height;
        m_texture.LoadFromMemory(width, height, 4, 8, nullptr);
        m_texture.SetMinMagFilter(Texture::Linear, Texture::Linear);
    }

    bool RenderTexture::SetActive(bool active)
    {
        if(active)
        {
            m_frameBuffer->Bind();
            return m_frameBuffer->AttachTexture(m_texture);
        }
        else
        {
            m_frameBuffer->Unbind();
            return true;
        }
    }

    void RenderTexture::Clear() noexcept
    {
        PushState();
        SetActive(true);
        SetView({0,0}, {m_width, m_height});
        SetViewport({0,0}, {m_width, m_height});
        ResetCamera();
        RenderTarget::Clear();
    }

    void RenderTexture::Clear(Color color) noexcept
    {
        PushState();
        SetActive(true);
        SetView({0,0}, {m_width, m_height});
        SetViewport({0,0}, {m_width, m_height});
        ResetCamera();
        RenderTarget::Clear(color);
    }

    void RenderTexture::Display() noexcept
    {
        RenderTarget::EndBatch();
        SetActive(false);
        PopState();
    }

    const Texture& RenderTexture::GetTexture() const noexcept
    {
        return m_texture;
    }
}