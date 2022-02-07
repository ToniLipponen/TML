#include <TML/Interface/Components/TextInput.h>
#include <TML/IO/Clipboard.h>

using namespace tml::Interface;

TextInput::TextInput(i32 x, i32 y, i32 width, i32 height)
{
    m_pos = Vector2i(x,y);
    m_size = Vector2i(width, height);
    m_text.SetPosition(m_pos);
    m_text.SetColor(Color::Black);
    m_cursorPos = Math::Clamp<int>(m_pos.x + 2, m_pos.x, m_pos.x + m_size.x - 4);

    m_hSizePolicy = Expand;
    m_vSizePolicy = Fixed;
    InitListeners();
}

void TextInput::SetValue(const std::string &string)
{
    m_value = Util::StringToWstring(string);
    m_text.SetString(string);
    m_cursorIndex = string.length();
    m_cursorPos = m_text.GetDimensions().x;
}

void TextInput::InitListeners()
{
    static auto search_forwards = [&](){
        ui32 index = m_cursorIndex;
        for(;index < m_value.size()-1; index++)
            if(m_value.at(index) == ' ')
                break;
        return index;
    };
    static auto search_backwards = [&](){
        ui32 index = m_cursorIndex-1;
        for(;index > 0; index--)
            if(m_value.at(index) == ' ')
                break;
        return index;
    };

    AddListener("MouseDown", [&](BaseComponent* c, Event& e)
    {
        if(m_state.MouseOver)
        {
            m_state.MouseDown = e.mouseButton.button;
            e = Event{};
        }
        else
            UnFocus();
    });
    AddListener("Click", [&](BaseComponent* c, Event& e)
    {
        if(m_state.MouseOver)
        {
            Focus();
            Raise();
            e = Event{};
        }
        else
            UnFocus();
    });
    AddListener("InterfaceUpdate", [&](BaseComponent* c, Event& e)
    {
        if(m_state.Focused)
        {
            if((m_blinkTimer += e.update.delta) > 1.0)
            {
                m_showLine = !m_showLine;
                m_blinkTimer = 0;
            }
        }
    });

    AddListener("KeyPressed", [&](BaseComponent* c, Event& e)
    {
        if(m_state.Focused)
        {
            switch(e.key.code)
            {
                case Keyboard::KEY_LEFT:
                    if(e.key.control)
                        m_cursorIndex = search_backwards();
                    else
                        m_cursorIndex = Math::Clamp<i32>(--m_cursorIndex, 0, m_value.size());
                    break;

                case Keyboard::KEY_RIGHT:
                    if(e.key.control)
                        m_cursorIndex = search_forwards();
                    else
                        m_cursorIndex = Math::Clamp<i32>(++m_cursorIndex, 0, m_value.size());
                    break;

                case Keyboard::KEY_BACKSPACE:
                    if(m_cursorIndex != 0)
                    {
                        if(e.key.control)
                        {
                            const ui32 index = search_backwards();
                            m_value.erase(m_value.begin() + index, m_value.begin() + m_cursorIndex);
                            m_cursorIndex -= (m_cursorIndex - index);
                        }
                        else
                        {
                            m_value.erase(m_value.begin() + m_cursorIndex - 1);
                            m_cursorIndex = Math::Clamp<ui32>(--m_cursorIndex, 0, m_value.size());
                        }
                    }
                    break;
                case Keyboard::KEY_DELETE:
                    if(m_cursorIndex != m_value.size() - 1)
                    {
                        if(e.key.control)
                        {
                            const ui32 index = search_forwards();
                            m_value.erase(m_value.begin() + m_cursorIndex, m_value.begin() + index);
                        }
                        else
                        {
                            m_value.erase(m_value.begin() + m_cursorIndex);
                        }
                    }
                    break;
                case Keyboard::KEY_V:
                    if(e.key.control)
                    {
                        const auto str = Clipboard::GetString();
                        m_value.insert(m_cursorIndex, Util::StringToWstring(Clipboard::GetString()));
                        m_cursorIndex += str.length();
                    }
                    break;
                default:
                    break;
            }
            m_text.SetString(m_value.substr(0, m_cursorIndex));
            m_cursorPos = Math::Clamp<float>(m_pos.x + m_text.GetDimensions().x + 2, m_pos.x, m_pos.x + m_size.x - 4);
            m_text.SetString(m_value);
        }
    });

    AddListener("TextEntered", [&](BaseComponent* c, Event& e)
    {
        if(m_state.Focused)
        {
            m_value.insert(m_cursorIndex, 1, static_cast<wchar_t>(e.text.unicode));
            m_cursorIndex++;
            m_text.SetString(m_value.substr(0, m_cursorIndex));
            m_cursorPos = Math::Clamp<float>(m_pos.x + m_text.GetDimensions().x + 2, m_pos.x, m_pos.x + m_size.x - 4);
            m_text.SetString(m_value);
        }
    });

    AddListener("Moved", [&](BaseComponent* c, Event& e)
    {
        m_text.SetPosition(e.move.x, e.move.y);
        m_cursorPos = Math::Clamp<float>(m_pos.x + m_text.GetDimensions().x + 2, m_pos.x, m_pos.x + m_size.x - 4);
    });
}

void TextInput::Draw(RenderWindow& window)
{
    window.DrawRect(m_pos, m_size, m_pColor);
    window.SetBounds(m_pos, m_size);
    window.Draw(m_text);

    if(m_state.Focused)
    {
        if(m_showLine)
            window.DrawLine({m_cursorPos, m_pos.y + (m_size.y / 10.0f)}, {m_cursorPos, m_pos.y + m_size.y - (m_size.y / 10.f)}, 2, Color::Black, 0);

        window.ResetBounds();
        window.DrawGrid(m_pos, m_size, 1, 1, m_activeColor, 1);
    }
    else
    {
        window.ResetBounds();
        window.DrawGrid(m_pos, m_size, 1, 1, m_sColor, 1);
    }
}