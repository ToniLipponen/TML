#include <TML/Interface/Components/Dropmenu.h>
#include <TML/Renderer.h>

#include <utility>

extern tml::Text* DEFAULT_TEXT;
namespace tml
{
    namespace Interface
    {
        DropMenu::DropMenu(i32 x, i32 y, ui32 width, ui32 height)
        {
            m_pos = Vector2(x,y);
            m_size = Vector2(width, height);
            AddChild(m_listComponent = new Listbox(x, y + height + 2, width, 0));
            m_listComponent->Disable();
        }

        void DropMenu::AddValue(std::string value)
        {
            m_listComponent->AddValue(std::move(value));
            m_listComponent->SetSize(Vector2(m_size.x, m_listComponent->GetElementsCount() * 20.f));
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
            Renderer::DrawText(selected_value, m_pos, m_size.y, BLACK);

            const Vector2 pos = m_pos + Vector2(1,1);
            const Vector2 size = m_size - Vector2(2,2);
            if(m_state.Focused)
                Renderer::DrawGrid(pos, size, 1, 1, m_activeColor, 2);
            else
                Renderer::DrawGrid(pos, size, 1, 1, m_sColor, 2);

        }

        void DropMenu::OnMouseClick(const Vector2 &mousePos)
        {

        }

        void DropMenu::OnMouseDown(const Vector2 &mousePos)
        {

        }

        void DropMenu::OnUpdate(double dt)
        {

        }

        void DropMenu::OnFocusLost()
        {
            m_listComponent->Disable();
        }

        void DropMenu::OnFocused()
        {
            m_listComponent->Enable();
        }
    }
}