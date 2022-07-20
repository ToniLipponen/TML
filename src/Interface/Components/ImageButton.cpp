#include <TML/Interface/Components/ImageButton.h>

namespace tml::Interface
{
    ImageButton::ImageButton(const Image& image, uint32_t h, int32_t x, int32_t y) noexcept
    : Component(x, y, h * (image.GetWidth() / image.GetHeight()), h)
    {
        Image img = image;
        img.FlipVertically();
        m_texture.LoadFromImage(img);
        m_texture.SetSize(m_size);

        AddListener("Moved", [&](Component* c, const Event& e)
        {
            m_texture.SetPosition(m_pos);
            return true;
        });

        AddListener("Resized", [&](Component* c, const Event& e)
        {
            m_texture.SetSize(m_size);
            return true;
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
