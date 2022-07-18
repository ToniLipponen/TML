#pragma once
#include <TML/Interface/Component.h>
#include <TML/System/Math/Vector2.h>
#include <TML/Export.h>

namespace tml::Interface
{
    class TML_API Layout : public Component
    {
    public:
        Layout() noexcept;
        void SetPadding(const Vector2i& padding) noexcept;
        Vector2i GetPadding() const noexcept;

    private:
        void pDraw(RenderTarget& target) noexcept override;

    protected:
        virtual void ScaleChildren() noexcept = 0;
        virtual void AlignChildren() noexcept = 0;

    protected:
        Vector2i m_padding = {5, 5};
    };
}