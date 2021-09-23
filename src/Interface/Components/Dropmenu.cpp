#include <TML/Interface/Components/Dropmenu.h>
#include <TML/Renderer.h>

extern tml::Text* DEFAULT_TEXT;
namespace tml
{
    namespace Interface
    {
        DropMenu::DropMenu(i32 x, i32 y, ui32 width, ui32 height)
        {
            m_absPos = Vector2(x,y);
            m_relPos = m_absPos;

            m_absSize = Vector2(width, height);
            m_relSize = m_absSize;
            AddChild(new Listbox(x, y + height + 2, width, 0));
            m_child->Disable();
        }

        void DropMenu::AddValue(std::string value)
        {
            ((Listbox*)m_child)->AddValue(value);
            ((Listbox*)m_child)->SetSize(Vector2(m_absSize.x, ((Listbox*)m_child)->GetElementsCount() * 20.f));
        }

        void DropMenu::SetValue(ui32 index, std::string value)
        {
            ((Listbox*)m_child)->SetValue(index, value);
        }

        std::string DropMenu::GetValue(ui32 index)
        {
            return ((Listbox*)m_child)->GetValue(index);
        }

        std::string DropMenu::GetSelectedValue() const
        {
            return ((Listbox*)m_child)->GetSelectedValue();
        }

        tml::i32 DropMenu::GetSelectedIndex() const
        {
            return ((Listbox*)m_child)->GetSelectedIndex();
        }

        void DropMenu::Clear()
        {
            ((Listbox*)m_child)->Clear();
        }

        void DropMenu::Draw()
        {
            std::string selected_value;
            if(m_child)
                selected_value = ((Listbox*)m_child)->GetSelectedValue();

            Renderer::DrawRect(m_absPos, m_absSize, m_pColor);
            Renderer::DrawText(selected_value, m_absPos, m_absSize.y, BLACK);
            if(ActiveComponent == this)
                Renderer::DrawGrid(m_absPos, m_absSize, 1, 1, m_activeColor, 2);
            else
                Renderer::DrawGrid(m_absPos, m_absSize, 1, 1, m_sColor, 2);

        }

        void DropMenu::OnMouseClick(const Vector2 &mousePos)
        {

        }

        void DropMenu::OnMouseDown(const Vector2 &mousePos)
        {

        }

        void DropMenu::OnUpdate(float dt)
        {

        }

        void DropMenu::OnFocusLost()
        {
            m_child->Disable();
        }

        void DropMenu::OnFocused()
        {
            m_child->Enable();
        }
    }
}