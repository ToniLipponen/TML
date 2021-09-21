#include <TML/Interface/Components/TextInput.h>
#include <TML/Renderer.h>
#include <TML/IO/Clipboard.h>

// Sets a given bit
#define SETBIT(value, bit, state) (value ^= (-state ^ value) & (1UL << bit))
// Checks whether a bit is set or not
#define BITSET(value, bit) ((value & bit) > 0)

using namespace tml::Interface;
extern tml::Text* DEFAULT_TEXT;

TextInput::TextInput(i32 x, i32 y, i32 width, i32 height)
{
    m_relPos = Vector2(x,y);
    m_absPos = m_relPos;

    m_relSize = Vector2(width, height);
    m_absSize = m_relSize;
}

void TextInput::OnMouseClick(const Vector2 &p)
{
    Keyboard::BeginString();
}

void TextInput::OnUpdate(float dt)
{
    m_repeatTimer = Util::Max(m_repeatTimer += dt, 0.11f);
    if(ActiveComponent == this)
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
    DEFAULT_TEXT->SetPosition(m_absPos);
    DEFAULT_TEXT->SetString(m_value);
    Renderer::DrawRect(m_absPos, m_absSize, m_pColor);
//    Renderer::DrawText(m_value, m_absPos, m_absSize.y, BLACK);
    Renderer::DrawTextCropped(m_value, m_absPos,m_absSize.y, BLACK, m_absPos, m_absPos + m_absSize);
    if(ActiveComponent == this)
        Renderer::DrawGrid(m_absPos, m_absSize, 1, 1,m_activeColor, 2);
    else
        Renderer::DrawGrid(m_absPos, m_absSize, 1, 1,m_sColor,2);
}