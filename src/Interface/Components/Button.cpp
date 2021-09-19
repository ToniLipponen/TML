#include <TML/Interface/Components/Button.h>
#include <TML/Renderer.h>

using namespace tml::Interface;
extern tml::Text* DEFAULT_TEXT;

Button::Button(ui32 x, ui32 y, ui32 w, ui32 h)
{
    m_absSize = Vector2(w,h);
    m_absPos = Vector2(x,y);
    m_relPos = m_absPos;
    m_relSize = m_absSize;
}

void Button::SetText(const std::string &str)
{
    m_text = str;
}

void Button::Draw()
{
    DEFAULT_TEXT->SetPosition({0,0});
    DEFAULT_TEXT->SetString(m_text);
    DEFAULT_TEXT->SetSize(m_absSize.y * (2.f / 3.f));
    Vector2 size = DEFAULT_TEXT->GetDimensions();

    if((m_eventStatus & Click) > 0)
        Renderer::DrawRect(m_absPos, m_absSize, RED);
    else
        Renderer::DrawRect(m_absPos, m_absSize, m_pColor);
    Renderer::DrawText(m_text, m_absPos + (m_absSize * (2.f / 3.f)) - (size * (2.f / 3.f)), m_absSize.y  * (2.f / 3.f), BLACK);
    Renderer::DrawGrid(m_absPos, m_absSize, 1, 1, BLUE);
}