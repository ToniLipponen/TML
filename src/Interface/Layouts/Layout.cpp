#include <TML/Interface/Layout.h>

namespace tml::Interface
{
    Layout::Layout() noexcept
    {
        m_hSizePolicy = SizePolicy::Expand;
        m_vSizePolicy = SizePolicy::Expand;

        AddListener("ChildAdded", [&](BaseComponent* c, Event& e)
        {
            ScaleChildren();
            AlignChildren();
        });

        AddListener("Resized", [&](BaseComponent* c, Event& e)
        {
            ScaleChildren();
            AlignChildren();
        });

        AddListener("Moved", [&](BaseComponent* c, Event& e)
        {
            ScaleChildren();
            AlignChildren();
        });

        AddListener("Attached", [&](BaseComponent* c, Event& e)
        {
            ScaleChildren();
            AlignChildren();
        });
    }

    void Layout::SetPadding(const Vector2i &padding) noexcept
    {
        m_padding = padding;
    }

    Vector2i Layout::GetPadding() const noexcept
    {
        return m_padding;
    }

    void Layout::pDraw(RenderTarget& target) noexcept
    {
        target.DrawRect(m_pos, m_size, m_sColor);
    }
}