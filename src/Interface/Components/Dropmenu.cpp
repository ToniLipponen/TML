#include <TML/Interface/Components/Dropmenu.h>
#include <TML/Renderer.h>

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
            m_list = Listbox(x, y + height, width, 0);
        }

        void DropMenu::AddValue(std::string value)
        {
            m_list.AddValue(value);
            m_list.SetSize(Vector2(m_absSize.x, m_list.GetElementsCount() * 20.f));
        }

        void DropMenu::SetValue(ui32 index, std::string value)
        {
            m_list.SetValue(index, value);
        }

        std::string DropMenu::GetValue(ui32 index)
        {
            return m_list.GetValue(index);
        }

        std::string DropMenu::GetSelectedValue() const
        {
            return m_list.GetSelectedValue();
        }

        tml::i32 DropMenu::GetSelectedIndex() const
        {
            return m_list.GetSelectedIndex();
        }

        void DropMenu::Clear()
        {
            m_list.Clear();
        }

        void DropMenu::Draw()
        {
            const auto& selected_value = m_list.GetSelectedValue();
            Renderer::DrawRect(m_absPos, m_absSize, m_pColor);
            Renderer::DrawText(selected_value, m_absPos, m_absSize.y, BLACK);
            if(ActiveComponent == this)
                Renderer::DrawGrid(m_absPos, m_absSize, 1, 1, m_activeColor, 2);
            else
                Renderer::DrawGrid(m_absPos, m_absSize, 1, 1, m_sColor, 2);

        }

        void DropMenu::OnUpdate(float dt)
        {
            if(ActiveComponent == this)
            {
                m_list.Update(dt);
                printf("Dropmenu update\n");
            }
        }
    }
}