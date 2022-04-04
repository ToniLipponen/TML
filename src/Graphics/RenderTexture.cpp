#include <TML/Graphics/RenderTexture.h>
#include <TML/Graphics/Core/FrameBuffer.h>

namespace tml
{
    RenderTexture::RenderTexture()
    : m_frameBuffer(nullptr)
    {

    }

    RenderTexture::~RenderTexture() noexcept
    {
        delete m_frameBuffer;
    }

    void RenderTexture::Create(i32 width, i32 height) noexcept
    {
        if(!m_frameBuffer)
            m_frameBuffer = new FrameBuffer();
        
        m_width = width;
        m_height = height;
        m_texture.LoadFromMemory(width, height, 4, nullptr);
        m_texture.SetMinMagFilter(Texture::Nearest, Texture::Nearest);
        m_texture.SetClampMode(Texture::Repeat);
    }

    bool RenderTexture::SetActive(bool active)
    {
        if(active)
        {
            m_texture.SetMinMagFilter(Texture::Nearest, Texture::Nearest);
            auto returnValue = m_frameBuffer->AttachTexture(m_texture);
            m_frameBuffer->Bind();
            return returnValue;
        }
        else
        {
            m_frameBuffer->Unbind();
            return true;
        }
    }

    void RenderTexture::Clear() noexcept
    {
        SetActive();
        SetViewport({0,0}, {m_width, m_height});
        SetView({0,0}, {m_width, m_height});
        Renderer::Clear();
    }

    void RenderTexture::Display() noexcept
    {
        Renderer::EndBatch();
        SetActive(false);
        m_texture.SetMinMagFilter(Texture::Linear, Texture::Linear);
    }

    const Texture& RenderTexture::GetTexture() const noexcept
    {
        return m_texture;
    }
}