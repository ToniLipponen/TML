#include <TML/Interface/Components/Menubar.h>

namespace tml::Interface
{
    Menubar::Menubar(RenderWindow &window) noexcept
    : HorizontalLayout(0,0, window.GetWidth(), 20)
    {
        m_vSizePolicy = SizePolicy::Fixed;
        m_padding.x = 1;

        AddListener("WindowResized", [](BaseComponent* c, Event& e)
        {
            c->SetSize(e.size.w, static_cast<uint32_t>(c->GetSize().y));
        });
    }

    void Menubar::pDraw(RenderTarget& target) noexcept
    {
        target.DrawRect(m_pos, m_size, m_pColor);
        target.DrawLine(m_pos + Vector2f(0, m_size.y), m_pos + m_size, 1, m_sColor, false);
    }
}