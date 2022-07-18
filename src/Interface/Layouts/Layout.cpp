#include <TML/Interface/Layout.h>

namespace tml::Interface
{
    Layout::Layout() noexcept
    {
        m_hSizePolicy = SizePolicy::Expand;
        m_vSizePolicy = SizePolicy::Expand;

        AddListener("ChildAdded", [&](BaseComponent* c, const Event& e)
        {
            ScaleChildren();
            AlignChildren();
            return true;
        });

        AddListener("Resized", [&](BaseComponent* c, const Event& e)
        {
            ScaleChildren();
            AlignChildren();
            return true;
        });

        AddListener("Moved", [&](BaseComponent* c, const Event& e)
        {
            ScaleChildren();
            AlignChildren();
            return true;
        });

        AddListener("Attached", [&](BaseComponent* c, const Event& e)
        {
            ScaleChildren();
            AlignChildren();
            return true;
        });

        AddListener("MouseMoved", [&](BaseComponent* c, const Event& e)
        {
            return ContainsPoint(Vector2i(e.pos.x, e.pos.y));
        });

        AddListener("Dragged", [&](BaseComponent* c, const Event& e)
        {
            return ContainsPoint({e.drag.beginX, e.drag.beginY});
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