#include <TML/Interface/Components/DropList.h>
#include <utility>

namespace tml
{
    namespace Interface
    {
        DropList::DropList(i32 x, i32 y, ui32 width, ui32 height)
        : BaseComponent(x,y,width,height)
        {
            AddChild(m_listComponent = new Listbox(x, y + height + 2, width, 200));
            m_listComponent->Disable();
            m_listComponent->AddListener("Click", [](BaseComponent* c, Event& e){ c->Disable(); });

            AddListener("MouseDown", [&](BaseComponent*, Event& e){
                if(m_state.MouseOver)
                {
                    m_state.MouseDown = e.mouseButton.button;
                    e = Event{};
                }
                else
                    m_listComponent->Disable();
            });
            AddListener("Click", [&](BaseComponent* c, Event& e)
            {
                if(m_state.MouseOver)
                {
                    m_listComponent->ToggleEnabled();
                    m_listComponent->Raise();
                    e = Event{};
                }
            });
            AddListener("Moved", [&](BaseComponent* c, Event& e)
            {
                m_listComponent->SetPosition({m_pos.x, m_pos.y + m_size.y + 2});
            });
        }

        void DropList::AddValue(String value)
        {
            m_listComponent->AddValue(std::move(value));
        }

        void DropList::SetValue(ui32 index, String value)
        {
            m_listComponent->SetValue(index, value);
        }

        void DropList::SetListHeight(int height)
        {
            m_listComponent->SetSize({m_size.x, height});
        }

        String DropList::GetValue(ui32 index)
        {
            return m_listComponent->GetValue(index);
        }

        String DropList::GetSelectedValue() const
        {
            return m_listComponent->GetSelectedValue();
        }

        tml::i32 DropList::GetSelectedIndex() const
        {
            return m_listComponent->GetSelectedIndex();
        }

        void DropList::Clear()
        {
            m_listComponent->Clear();
        }

        void DropList::pDraw(Renderer &window)
        {
            String selected_value;
            if(m_listComponent)
                selected_value = m_listComponent->GetSelectedValue();

            window.DrawRect(m_pos, m_size, m_pColor);
            window.SetBounds(m_pos, m_size);
            window.DrawText(selected_value, m_pos, m_size.y, Color::Black);
            window.ResetBounds();

            if(m_state.Focused)
                window.DrawGrid(m_pos, m_size, 1, 1, m_activeColor, 1);
            else
                window.DrawGrid(m_pos, m_size, 1, 1, m_sColor, 1);
        }
    }
}