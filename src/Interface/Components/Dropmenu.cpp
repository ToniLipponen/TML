#include <TML/Interface/Components/Dropmenu.h>
#include <TML/Renderer.h>

extern tml::Text* DEFAULT_TEXT;
namespace tml::Interface
{
    DropMenu::DropMenu(i32 x, i32 y, ui32 width, ui32 height)
    {
        m_pos = Vector2(x,y);
        m_size = Vector2(width, height);
        AddChild(m_listComponent = new Listbox(x, y + height + 2, width, 0));
        m_listComponent->Disable();
    }

    void DropMenu::AddValue(std::wstring value)
    {
        ((Listbox*)m_listComponent)->AddValue(value);
        ((Listbox*)m_listComponent)->SetSize(Vector2(m_size.x, ((Listbox*)m_listComponent)->GetElementsCount() * 20.f));
    }

    void DropMenu::SetValue(ui32 index, std::wstring value)
    {
        ((Listbox*)m_listComponent)->SetValue(index, value);
    }

    std::wstring DropMenu::GetValue(ui32 index)
    {
        return ((Listbox*)m_listComponent)->GetValue(index);
    }

    std::wstring DropMenu::GetSelectedValue() const
    {
        return ((Listbox*)m_listComponent)->GetSelectedValue();
    }

    tml::i32 DropMenu::GetSelectedIndex() const
    {
        return ((Listbox*)m_listComponent)->GetSelectedIndex();
    }

    void DropMenu::Clear()
    {
        ((Listbox*)m_listComponent)->Clear();
    }

    void DropMenu::Draw()
    {
        std::wstring selected_value;
        if(m_listComponent)
            selected_value = ((Listbox*)m_listComponent)->GetSelectedValue();

        Renderer::DrawRect(m_pos, m_size, m_pColor);
        Renderer::DrawText(selected_value, m_pos, m_size.y, BLACK);
        if(s_activeComponent == this)
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
        m_listComponent->Disable();
    }

    void DropMenu::OnFocused()
    {
        m_listComponent->Enable();
    }
}