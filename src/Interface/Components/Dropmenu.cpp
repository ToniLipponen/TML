#include <TML/Interface/Components/Dropmenu.h>
#include <TML/Renderer.h>

extern tml::Text* DEFAULT_TEXT;
namespace tml::Interface
{
    DropMenu::DropMenu(i32 x, i32 y, ui32 width, ui32 height)
    {
        m_pos = Vector2(x,y);
        m_size = Vector2(width, height);
        AddChild(new Listbox(x, y + height + 2, width, 0));
        m_child->Disable();
    }

    void DropMenu::AddValue(std::wstring value)
    {
        ((Listbox*)m_child)->AddValue(value);
        ((Listbox*)m_child)->SetSize(Vector2(m_size.x, ((Listbox*)m_child)->GetElementsCount() * 20.f));
    }

    void DropMenu::SetValue(ui32 index, std::wstring value)
    {
        ((Listbox*)m_child)->SetValue(index, value);
    }

    std::wstring DropMenu::GetValue(ui32 index)
    {
        return ((Listbox*)m_child)->GetValue(index);
    }

    std::wstring DropMenu::GetSelectedValue() const
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
        std::wstring selected_value;
        if(m_child)
            selected_value = ((Listbox*)m_child)->GetSelectedValue();

        Renderer::DrawRect(m_pos, m_size, m_pColor);
        Renderer::DrawText(selected_value, m_pos, m_size.y, BLACK);
        if(m_state.Focused)
            Renderer::DrawGrid(m_pos, m_size, 1, 1, m_activeColor, 2);
        else
            Renderer::DrawGrid(m_pos, m_size, 1, 1, m_sColor, 2);

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