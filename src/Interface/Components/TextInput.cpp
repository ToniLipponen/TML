#include <TML/Interface/Components/TextInput.h>
#include <TML/Renderer.h>
#include <TML/IO/Clipboard.h>

using namespace tml::Interface;
extern tml::Text* DEFAULT_TEXT;

TextInput::TextInput(i32 x, i32 y, i32 width, i32 height)
{
    m_pos = Vector2(x,y);
    m_size = Vector2(width, height);

    m_hSizePolicy = Expand;
    m_vSizePolicy = Fixed;
}

void TextInput::OnMouseClick(const Vector2 &p)
{
    Keyboard::BeginString();
}

void TextInput::OnUpdate(double dt)
{
    if(m_state.Focused)
    {
        if(Keyboard::IsKeyPressed(Keyboard::KEY_LEFT))
            m_cursorIndex--;
        else if(Keyboard::IsKeyPressed(Keyboard::KEY_RIGHT))
            m_cursorIndex++;
        if(Keyboard::IsKeyDown(Keyboard::KEY_LEFT_CONTROL))
        {
            auto search_forwards = [&](){
                ui32 index = m_cursorIndex;
                for(;index < m_value.size()-1; index++)
                    if(m_value.at(index) == ' ')
                        break;
                return index;
            };
            auto search_backwards = [&](){
                ui32 index = m_cursorIndex-1;
                for(;index > 0; index--)
                    if(m_value.at(index) == ' ')
                        break;
                return index;
            };
            if(Keyboard::IsKeyPressed(Keyboard::KEY_V) && !Clipboard::IsEmpty())
            {
                const auto str = Util::StringToWstring(Clipboard::GetString());
                m_value.insert(m_cursorIndex, str);
                m_cursorIndex += str.size();
            }
            else if(Keyboard::IsKeyPressed(Keyboard::KEY_BACKSPACE) && !m_value.empty())
            {
                ui32 index = search_backwards();
                m_value.erase(m_value.begin() + index, m_value.begin() + m_cursorIndex);
                m_cursorIndex -= (m_cursorIndex - index);
            }
            else if(Keyboard::IsKeyPressed(Keyboard::KEY_DELETE) && !m_value.empty())
            {
                ui32 index = search_forwards();
                m_value.erase(m_value.begin() + m_cursorIndex, m_value.begin() + Util::Max<ui32>(index + 1, m_value.size()));
            }
            else if(Keyboard::IsKeyPressed(Keyboard::KEY_LEFT))
                m_cursorIndex = search_backwards();
            else if(Keyboard::IsKeyPressed(Keyboard::KEY_RIGHT))
                m_cursorIndex = search_forwards();
        }
        else
        {
            if(Keyboard::IsKeyPressed(Keyboard::KEY_BACKSPACE) && !m_value.empty())
            {
                m_value.erase(Util::Clamp<ui32>(m_cursorIndex-1, 0, m_value.length() -1), 1);
                m_cursorIndex--;
            }
            else if(Keyboard::IsKeyPressed(Keyboard::KEY_DELETE) && !m_value.empty())
                m_value.erase(m_cursorIndex, 1);
        }

        const auto str = Util::StringToWstring(Keyboard::EndString());
        if(!str.empty())
        {
            m_value.insert(m_cursorIndex, str);
            m_cursorIndex += str.size();
        }
        m_cursorIndex = Util::Clamp<ui32>(m_cursorIndex, 0, m_value.size());
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
    DEFAULT_TEXT->SetSize(m_size.y);
    DEFAULT_TEXT->SetString(m_value.substr(0, m_cursorIndex));
    const auto cursorX = Util::Clamp(m_pos.x + DEFAULT_TEXT->GetDimensions().x + 2, m_pos.x, m_pos.x + m_size.x);
    DEFAULT_TEXT->SetString(m_value);

    Renderer::DrawRect(m_pos, m_size, m_pColor);
    Renderer::DrawTextCropped(Util::WstringToString(m_value), m_pos, m_size.y, BLACK, m_pos, m_pos + m_size);
    if(m_state.Focused)
    {
        if(m_showLine)
            Renderer::DrawLine({cursorX, m_pos.y + (m_size.y / 10.0f)}, {cursorX, m_pos.y + m_size.y - (m_size.y / 10.f)}, 2, BLACK, 0);
        Renderer::DrawGrid(m_pos, m_size, 1, 1, m_activeColor, 2);
    }
    else
        Renderer::DrawGrid(m_pos, m_size, 1, 1, m_sColor, 2);
}