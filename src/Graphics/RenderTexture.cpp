#include <TML/Graphics/RenderTexture.h>
#include <TML/Graphics/Core/RenderTarget.h>

namespace tml
{
    RenderTexture::RenderTexture()
    : m_target(nullptr)
    {

    }

    RenderTexture::~RenderTexture() noexcept
    {
        delete m_target;
    }

    bool RenderTexture::Create(ui32 width, ui32 height) noexcept
    {
        if(!m_target)
            m_target = new RenderTarget();

        m_texture.LoadFromMemory(width, height, 4, nullptr);
        m_texture.SetMinMagFilter(Texture::Nearest, Texture::Nearest);
    }

    bool RenderTexture::SetActive(bool active)
    {
        if(active)
        {
            return m_target->AttachTexture(m_texture);
        }
        else
        {
            m_target->Unbind();
            return true;
        }
    }

    void RenderTexture::Display() noexcept
    {
        Renderer::EndBatch();
        SetActive(false);
    }

    const Texture& RenderTexture::GetTexture() const noexcept
    {
        return m_texture;
    }
}