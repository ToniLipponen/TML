#include <TML/Interface/Components/TextInput.h>
#include <TML/Graphics/Renderer.h>
#include <TML/IO/Clipboard.h>

using namespace tml::Interface;
extern tml::Text* DEFAULT_TEXT;

TextInput::TextInput(i32 x, i32 y, i32 width, i32 height)
{
    m_pos = Vector2i(x,y);
    m_size = Vector2i(width, height);

    m_hSizePolicy = Expand;
    m_vSizePolicy = Fixed;
    InitListeners();
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

    AddListener("iMouseDown", [&](BaseComponent* c, Event& e)
    {
        if(m_state.MouseOver)
        {
            m_state.MouseDown = e.mouseButton.button;
            e = Event{};
            return true;
        }
        else
        {
            UnFocus();
            return false;
        }
    });
    AddListener("iClick", [&](BaseComponent* c, Event& e)
    {
        if(m_state.MouseOver)
        {
            Focus();
            e = Event{};
            return true;
        }
        return false;
    });

    AddListener("iKeyPressed", [&](BaseComponent* c, Event& e)
    {
        if(m_state.Focused) {
            switch(e.key.code) {
                case Keyboard::KEY_LEFT:
                    if(e.key.control)
                        m_cursorIndex = search_backwards();
                    else
                        m_cursorIndex--;
                    break;

                case Keyboard::KEY_RIGHT:
                    if(e.key.control)
                        m_cursorIndex = search_forwards();
                    else
                        m_cursorIndex = Math::Clamp<ui32>(++m_cursorIndex, 0, m_value.size() - 1);
                    break;

                case Keyboard::KEY_BACKSPACE:
                    if(m_cursorIndex != 0) {
                        if(e.key.control) {
                            const ui32 index = search_backwards();
                            m_value.erase(m_value.begin() + index, m_value.begin() + m_cursorIndex);
                            m_cursorIndex -= (m_cursorIndex - index);
                        }
                        else {
                            m_value.erase(m_value.begin() + m_cursorIndex - 1);
                            m_cursorIndex = Math::Clamp<ui32>(--m_cursorIndex, 0, m_value.size());
                        }
                    }
                    break;
                case Keyboard::KEY_DELETE:
                    if(m_cursorIndex != m_value.size() - 1) {
                        if(e.key.control) {
                            const ui32 index = search_forwards();
                            m_value.erase(m_value.begin() + m_cursorIndex, m_value.begin() + index);
                        }
                        else {
                            m_value.erase(m_value.begin() + m_cursorIndex);
                        }
                    }
                    break;

                default:
                    break;
            }
            return true;
        }
        return false;
    });

    AddListener("iTextEntered", [&](BaseComponent* c, Event& e)
    {
        if(m_state.Focused)
        {
            m_value.insert(m_value.begin() + m_cursorIndex, e.text.unicode);
            m_cursorIndex++;
            return true;
        }
        return false;
    });
}

void TextInput::OnUpdate(double dt)
{
    if(m_state.Focused)
    {
        if((m_blinkTimer += dt) > 1.0)
        {
            m_showLine = !m_showLine;
            m_blinkTimer = 0;
        }
    }
}

void TextInput::Draw()
{
    // Setting DEFAULT_TEXT values here, so I can measure the dimensions of the text.
    DEFAULT_TEXT->SetPosition(m_pos);
    DEFAULT_TEXT->SetSize(m_size.y * 0.8);
    DEFAULT_TEXT->SetString(m_value.substr(0, m_cursorIndex));
    const float cursorX = Math::Clamp<int>(m_pos.x + DEFAULT_TEXT->GetDimensions().x + 2, m_pos.x, m_pos.x + m_size.x - 4);

    DEFAULT_TEXT->SetString(m_value);

    Renderer::DrawRect(m_pos, m_size, m_pColor);
    Renderer::SetBounds(m_pos, m_size);
    Renderer::DrawText(Util::WstringToString(m_value), m_pos + Vector2i(0, m_size.y * 0.2f), m_size.y * 0.8, BLACK);

    if(m_state.Focused)
    {
        if(m_showLine)
            Renderer::DrawLine({cursorX, m_pos.y + (m_size.y / 10.0f)}, {cursorX, m_pos.y + m_size.y - (m_size.y / 10.f)}, 2, BLACK, 0);

        Renderer::ResetBounds();
        Renderer::DrawGrid(m_pos, m_size, 1, 1, m_activeColor, 1);
    }
    else
    {
        Renderer::ResetBounds();
        Renderer::DrawGrid(m_pos, m_size, 1, 1, m_sColor, 1);
    }
}