#include <TML/Interface/Components/WindowContainer.h>

namespace tml
{
    namespace Interface
    {
        WindowContainer::WindowContainer(RenderWindow& window)
        : BaseComponent(0,0, window.GetWidth(), window.GetHeight())
        {
            m_pColor = Color::White;
            AddListener("WindowResized", [&](BaseComponent* c, Event& e)
            {
               c->SetSize({e.size.x, e.size.y});
               if(!m_children.empty())
                   m_children.at(0)->SetSize(m_size);
            });
            AddListener("ChildAdded", [&](BaseComponent* c, Event& e)
            {
                m_children.at(0)->SetSize(m_size);
            });
        }

        void WindowContainer::Draw(RenderWindow& renderWindow)
        {
            renderWindow.DrawRect(m_pos, m_size, m_pColor);
        }
    }
}