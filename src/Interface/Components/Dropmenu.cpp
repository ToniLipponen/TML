#include <TML/Interface/Components/Dropmenu.h>
#include <utility>

namespace tml
{
    namespace Interface
    {
        Dropmenu::Dropmenu(i32 x, i32 y, ui32 width, ui32 height)
        {
            m_pos = Vector2i(x,y);
            m_size = Vector2i(width, height);
            AddChild(m_listComponent = new Listbox(x, y + height + 2, width, 0));
            m_listComponent->Disable();
            m_listComponent->AddListener("Click", [](BaseComponent* c, Event& e){
                c->Disable();
                e = Event{};
            });

            AddListener("MouseDown", [&](BaseComponent*, Event& e){
                if(m_state.MouseOver)
                {
                    m_state.MouseDown = e.mouseButton.button;
                    e = Event{};
                }
            });
            AddListener("Click", [&](BaseComponent* c, Event& e)
            {
                if(m_state.MouseOver)
                {
                    m_listComponent->ToggleEnabled();
                    m_listComponent->Raise();
                    e = Event{};
                }
                else
                    m_listComponent->Disable();
            });
            AddListener("Moved", [&](BaseComponent* c, Event& e)
            {
                m_listComponent->SetPosition({m_pos.x, m_pos.y + m_size.y + 2});
            });
        }

        void Dropmenu::AddValue(String value)
        {
            m_listComponent->AddValue(std::move(value));
            m_listComponent->SetSize(Vector2i(m_size.x, m_listComponent->GetElementsCount() * 20.f));
        }

        void Dropmenu::SetValue(ui32 index, String value)
        {
            m_listComponent->SetValue(index, value);
        }

        String Dropmenu::GetValue(ui32 index)
        {
            return m_listComponent->GetValue(index);
        }

        String Dropmenu::GetSelectedValue() const
        {
            return m_listComponent->GetSelectedValue();
        }

        tml::i32 Dropmenu::GetSelectedIndex() const
        {
            return m_listComponent->GetSelectedIndex();
        }

        void Dropmenu::Clear()
        {
            m_listComponent->Clear();
        }

        void Dropmenu::pDraw(Renderer &window)
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