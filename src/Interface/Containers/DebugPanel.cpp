#include <TML/Interface/Containers/DebugPanel.h>
#include <TML/Interface/Layouts/FreeForm.h>
#include <TML/Renderer.h>

namespace tml::Interface
{
    DebugPanel::DebugPanel(i32 x, i32 y, ui32 w, ui32 h, std::wstring title, Layout *layout)
    {
        m_pos = Vector2(x,y);
        m_size = Vector2(w,h);
        m_title = title;
        m_layout = ((layout == nullptr) ? new VerticalLayout(x,y,w,h) : layout );
    }

    void DebugPanel::Draw()
    {
        Renderer::DrawRect(m_pos, m_size, m_pColor, 10);
        Renderer::DrawText(m_title, m_pos + Vector2(10, 10), 20, BLACK);
    }

    void DebugPanel::OnMouseClick(const Vector2 &mousePos)
    {
        m_click = mousePos - m_pos;
    }

    void DebugPanel::OnMouseDrag(const Vector2 &mousePos)
    {
        if(Keyboard::IsKeyDown(Keyboard::KEY_LEFT_CONTROL))
            m_pos = mousePos - m_click;
    }

    void DebugPanel::OnUpdate(float dt)
    {
        m_layout->SetPosition(m_pos + Vector2(10,50));
        m_layout->SetSize(m_size - Vector2(20,20));
        m_layout->Update(0);
    }
}