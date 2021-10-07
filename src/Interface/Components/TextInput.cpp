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
    if(m_state.Focused)
    {
        if(Keyboard::IsKeyDown(Keyboard::KEY_LEFT_CONTROL)
        && Keyboard::IsKeyDown(Keyboard::KEY_V)
        && !Clipboard::IsEmpty())
            m_value = Clipboard::GetString();
        else if(Keyboard::IsKeyDown(Keyboard::KEY_BACKSPACE)
        && !m_value.empty()
        && m_repeatTimer > 0.1f)
        {
            m_value.pop_back();
            m_repeatTimer = 0;
        }
        else
            m_value += Keyboard::EndString();
    }
}

void TextInput::Draw()
{
    DEFAULT_TEXT->SetPosition(m_pos);
    DEFAULT_TEXT->SetString(m_value);
    Renderer::DrawRect(m_pos, m_size, m_pColor);
//    Renderer::DrawText(m_value, m_pos, m_size.y, BLACK);
    Renderer::DrawTextCropped(m_value, m_pos,m_size.y, BLACK, m_pos, m_pos + m_size);
    if(m_state.Focused)
        Renderer::DrawGrid(m_pos, m_size, 1, 1,m_activeColor, 2);
    else
        Renderer::DrawGrid(m_pos, m_size, 1, 1,m_sColor,2);
}