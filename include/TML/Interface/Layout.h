#pragma once
#include <TML/System/Math/Vector2.h>
#include <TML/Export.h>

namespace tml::Interface
{
    class TML_API Layout
    {
    public:
        void SetPadding(const Vector2i& padding) noexcept;
        Vector2i GetPadding() const noexcept;

    protected:
        Vector2i m_padding = {5, 5};
    };
}