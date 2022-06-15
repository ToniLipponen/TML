#include <TML/Interface/Components/ImageButton.h>

namespace tml::Interface
{
    ImageButton::ImageButton(const Image& image, uint32_t h, int32_t x, int32_t y) noexcept
    : BaseComponent(x, y, h * (image.GetWidth() / image.GetHeight()), h)
    {
        m_texture.LoadFromImage(image);
        m_texture.SetSize(m_size);

        AddListener("Click", [&](BaseComponent* c, Event& e)
        {
            UnFocus();
            e = Event{};
        });

        AddListener("MouseUp", [&](BaseComponent* c, Event& e)
        {
            if(!m_state.MouseOver)
            {
                UnFocus();
            }
        });

        AddListener("MouseDown",[&](BaseComponent* c, Event& e)
        {
            if(m_state.MouseOver)
            {
                Focus();
                m_state.MouseDown = static_cast<char>(e.mouseButton.button);
                e = Event{};
            }
            else
            {
                UnFocus();
            }
        });

        AddListener("Moved", [&](BaseComponent* c, Event& e)
        {
            m_texture.SetPosition(m_pos);
        });

        AddListener("Resized", [&](BaseComponent* c, Event& e)
        {
            m_texture.SetSize(m_size);
        });
    }

    void ImageButton::SetImage(const Image& image, uint32_t height) noexcept
    {
        const auto w = static_cast<float>(image.GetWidth());
        const auto h = static_cast<float>(image.GetHeight());

        m_texture.LoadFromImage(image);
        m_texture.SetSize((w / h) * h, h);
    }

    void ImageButton::pDraw(RenderTarget& renderTarget) noexcept
    {
        renderTarget.Draw(m_texture);
    }
}
