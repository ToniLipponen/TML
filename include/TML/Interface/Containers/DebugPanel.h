#pragma once
#include <TML/Interface/Container.h>
#include <TML/Interface/Layouts/Vertical.h>

namespace tml::Interface
{
    class DebugPanel : public Container
    {
    public:
        DebugPanel(i32 x, i32 y, ui32 w, ui32 h, std::wstring title, Layout* layout = nullptr);
    private:
        void Draw() override;
        void OnUpdate(float dt) override;
        void OnMouseClick(const Vector2& mp) override;
        void OnMouseDrag(const Vector2& mp) override;
        std::wstring m_title;
        Vector2 m_click = {0};
    };
}