#include <TML/Interface/Components/Combobox.h>
#include <utility>

namespace tml::Interface
{
    Combobox::Combobox(uint32_t width, uint32_t height, int32_t x, int32_t y) noexcept
    : Component(x, y, width, height)
    {
        AddChild(m_listComponent = new Listbox(width, 200, x, y + height + 2));
        AddChild(m_textInput = new LineInput(width, height, x, y));
        m_textInput->SetReadOnly(true);

        m_listComponent->Disable();
        m_listComponent->AddListener("Click", [](Component* c, const Event& e)
        {
            c->Disable();
            return true;
        });

        AddListener("LostFocus", [&](Component*, const Event& e)
        {
            m_listComponent->Disable();
            return true;
        });

        m_textInput->AddListener("Drawn", [&](Component*, const Event&)
        {
            /// This is stupid.
            if(m_textInput->GetValue() != m_listComponent->GetSelectedValue())
            {
                m_textInput->SetValue(m_listComponent->GetSelectedValue());
            }
            return true;
        });

        m_textInput->AddListener("Click", [&](Component* c, const Event& e)
        {
            if(m_state.Focused)
            {
                m_listComponent->ToggleEnabled();
                return true;
            }

            return true;
        });

        AddListener("Moved", [&](Component* c, const Event& e)
        {
            m_textInput->SetPosition(m_pos);
            m_listComponent->SetPosition({m_pos.x, m_pos.y + m_size.y + 2});
            return true;
        });
    }

    void Combobox::AddValue(String value) noexcept
    {
        m_listComponent->AddValue(std::move(value));
    }

    void Combobox::SetValue(uint32_t index, String value) noexcept
    {
        m_listComponent->SetValue(index, value);
    }

    void Combobox::SetListHeight(int height) noexcept
    {
        m_listComponent->SetSize(Vector2i(m_size.x, height));
    }

    String Combobox::GetValue(uint32_t index) noexcept
    {
        return m_listComponent->GetValue(index);
    }

    String Combobox::GetSelectedValue() const noexcept
    {
        return m_listComponent->GetSelectedValue();
    }

    int32_t Combobox::GetSelectedIndex() const noexcept
    {
        return m_listComponent->GetSelectedIndex();
    }

    void Combobox::Clear() noexcept
    {
        m_listComponent->Clear();
    }

    void Combobox::pDraw([[maybe_unused]] RenderTarget& target) noexcept
    {

    }
}