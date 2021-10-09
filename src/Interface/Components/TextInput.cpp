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
    m_vSizePolicy = Expand;
}

void TextInput::OnMouseClick(const Vector2 &p)
{
    Keyboard::BeginString();
}

void TextInput::OnUpdate(float dt)
{
    m_repeatTimer = Util::Max(m_repeatTimer += dt, 0.11f);
    if(m_state.Focused && m_repeatTimer > 0.1f)
    {
        if(Keyboard::IsKeyDown(Keyboard::KEY_LEFT_CONTROL)
        && Keyboard::IsKeyDown(Keyboard::KEY_V)
        && !Clipboard::IsEmpty())
            m_value = Clipboard::GetString();
        else if(Keyboard::IsKeyDown(Keyboard::KEY_BACKSPACE)
        && !m_value.empty())
        {
            m_value.erase(Util::Clamp<ui32>(m_cursorIndex-1, 0, m_value.length() -1), 1);
            m_cursorIndex--;
        }
        if(Keyboard::IsKeyDown(Keyboard::KEY_LEFT))
        {
            m_cursorIndex--;
        }
        else if(Keyboard::IsKeyDown(Keyboard::KEY_RIGHT))
        {
            m_cursorIndex++;
        }
        else
        {
            const auto str = Keyboard::EndString();
            if(!str.empty())
            {
                m_value.insert(m_cursorIndex, str);
                m_cursorIndex++;
            }
        }
        m_cursorIndex = Util::Clamp<ui32>(m_cursorIndex, 0, m_value.size());
        m_repeatTimer = 0;
    }
}

void TextInput::Draw()
{
    DEFAULT_TEXT->SetPosition(m_pos);
    DEFAULT_TEXT->SetSize(m_size.y);
    DEFAULT_TEXT->SetString(m_value.substr(0, m_cursorIndex));
    const auto cursorX = Util::Clamp(m_pos.x + DEFAULT_TEXT->GetDimensions().x + 5, m_pos.x, m_pos.x + m_size.x);
    DEFAULT_TEXT->SetString(m_value);

    Renderer::DrawRect(m_pos, m_size, m_pColor);
    Renderer::DrawTextCropped(m_value, m_pos,m_size.y, BLACK, m_pos, m_pos + m_size);
    Renderer::DrawLine({cursorX, m_pos.y + (m_size.y / 10.0f)}, {cursorX, m_pos.y + m_size.y - (m_size.y / 10.f)}, 2, BLACK, 0);
    if(m_state.Focused)
        Renderer::DrawGrid(m_pos, m_size, 1, 1, m_activeColor, 2);
    else
        Renderer::DrawGrid(m_pos, m_size, 1, 1, m_sColor, 2);
}