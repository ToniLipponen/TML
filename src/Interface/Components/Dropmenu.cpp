#include <TML/Interface/Components/Dropmenu.h>
#include <TML/Graphics/Renderer.h>

#include <utility>

extern tml::Text* DEFAULT_TEXT;
namespace tml
{
    namespace Interface
    {
        DropMenu::DropMenu(i32 x, i32 y, ui32 width, ui32 height)
        {
            m_pos = Vector2i(x,y);
            m_size = Vector2i(width, height);
            AddChild(m_listComponent = new Listbox(x, y + height + 2, width, 0));
            m_listComponent->Disable();

            AddListener("iClick", [&](BaseComponent* c, Event& e)
            {
                if(m_state.MouseOver)
                {
                    m_listComponent->ToggleEnabled();
                    return true;
                }
                return false;
            });
            AddListener("Click", [&](BaseComponent* c, Event& e)
            {
                m_listComponent->Disable();
                return false;
            });

            AddListener("iFocused", [&](BaseComponent* c, Event& e)
            {
                if(m_state.MouseOver)
                {
                    m_listComponent->Enable();
                    return true;
                }
                return false;
            });
        }

        void DropMenu::AddValue(std::string value)
        {
            m_listComponent->AddValue(std::move(value));
            m_listComponent->SetSize(Vector2i(m_size.x, m_listComponent->GetElementsCount() * 20.f));
        }

        void DropMenu::SetValue(ui32 index, std::string value)
        {
            m_listComponent->SetValue(index, value);
        }

        std::string DropMenu::GetValue(ui32 index)
        {
            return m_listComponent->GetValue(index);
        }

        std::string DropMenu::GetSelectedValue() const
        {
            return m_listComponent->GetSelectedValue();
        }

        tml::i32 DropMenu::GetSelectedIndex() const
        {
            return m_listComponent->GetSelectedIndex();
        }

        void DropMenu::Clear()
        {
            m_listComponent->Clear();
        }

        void DropMenu::Draw()
        {
            std::string selected_value;
            if(m_listComponent)
                selected_value = m_listComponent->GetSelectedValue();

            Renderer::DrawRect(m_pos, m_size, m_pColor);
            Renderer::SetBounds(m_pos, m_size);
            Renderer::DrawText(selected_value, m_pos, m_size.y, BLACK);
            Renderer::ResetBounds();

            if(m_state.Focused)
                Renderer::DrawGrid(m_pos, m_size, 1, 1, m_activeColor, 1);
            else
                Renderer::DrawGrid(m_pos, m_size, 1, 1, m_sColor, 1);

        }

        void DropMenu::OnUpdate(double dt)
        {

        }

        void DropMenu::OnMoved()
        {
            m_listComponent->SetPosition({m_pos.x, m_pos.y + m_size.y + 2});
        }

        void DropMenu::OnResized()
        {

        }
    }
}