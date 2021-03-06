#include <TML/Interface/Layout.h>

namespace tml::Interface
{
    Layout::Layout() noexcept
    {
        m_hSizePolicy = SizePolicy::Dynamic;
        m_vSizePolicy = SizePolicy::Dynamic;

        AddListener("ChildAdded", [&](Component* c, const Event& e)
        {
            ScaleChildren();
            AlignChildren();
            return true;
        });

        AddListener("Resized", [&](Component* c, const Event& e)
        {
            ScaleChildren();
            AlignChildren();
            return true;
        });

        AddListener("Moved", [&](Component* c, const Event& e)
        {
            ScaleChildren();
            AlignChildren();
            return true;
        });

        AddListener("Attached", [&](Component* c, const Event& e)
        {
            ScaleChildren();
            AlignChildren();
            return true;
        });

        AddListener("MouseMoved", [&](Component* c, const Event& e)
        {
            return ContainsPoint(Vector2i(e.pos.x, e.pos.y));
        });

        AddListener("Dragged", [&](Component* c, const Event& e)
        {
            return ContainsPoint({e.drag.beginX, e.drag.beginY});
        });
    }

    Vector2i Layout::SetPadding(const Vector2i &padding) noexcept
    {
        return m_padding = padding;
    }

    Vector2i Layout::SetMargin(const Vector2i& margin) noexcept
    {
        return m_margin = margin;
    }

    Vector2i Layout::GetPadding() const noexcept
    {
        return m_padding;
    }

    Vector2i Layout::GetMargin() const noexcept
    {
        return m_margin;
    }

    void Layout::pDraw(RenderTarget& target) noexcept
    {
        target.DrawRect(m_pos, m_size, m_sColor);
    }
}