#include <TML/Interface/Components/Combobox.h>
#include <utility>

namespace tml::Interface
{
    Combobox::Combobox(int32_t x, int32_t y, uint32_t width, uint32_t height)
    : BaseComponent(x,y,width,height)
    {
        AddChild(m_listComponent = new Listbox(x, y + height + 2, width, 200));
        AddChild(m_textInput = new TextInput(x, y, width, height));
        m_textInput->SetReadOnly(true);

        m_listComponent->Disable();
        m_listComponent->AddListener("Click", [](BaseComponent* c, Event& e){ c->Disable(); });

        AddListener("MouseDown", [&](BaseComponent*, Event& e)
        {
            if(m_state.MouseOver)
            {
                m_state.MouseDown = static_cast<char>(e.mouseButton.button);
                e = Event{};
            }
            else
            {
                m_listComponent->Disable();
            }
        });

        m_textInput->AddListener("Drawn", [&](BaseComponent*, Event&)
        {
            m_textInput->SetValue(String(m_listComponent->GetSelectedValue()));
        });

        m_textInput->AddListener("Click", [&](BaseComponent* c, Event& e)
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
            m_textInput->SetPosition(m_pos);
            m_listComponent->SetPosition({m_pos.x, m_pos.y + m_size.y + 2});
        });
    }

    void Combobox::AddValue(String value)
    {
        m_listComponent->AddValue(std::move(value));
    }

    void Combobox::SetValue(uint32_t index, String value)
    {
        m_listComponent->SetValue(index, value);
    }

    void Combobox::SetListHeight(int height)
    {
        m_listComponent->SetSize(Vector2i(m_size.x, height));
    }

    String Combobox::GetValue(uint32_t index)
    {
        return m_listComponent->GetValue(index);
    }

    String Combobox::GetSelectedValue() const
    {
        return m_listComponent->GetSelectedValue();
    }

    int32_t Combobox::GetSelectedIndex() const
    {
        return m_listComponent->GetSelectedIndex();
    }

    void Combobox::Clear()
    {
        m_listComponent->Clear();
    }

    void Combobox::pDraw([[maybe_unused]] RenderTarget& target) noexcept
    {

    }
}