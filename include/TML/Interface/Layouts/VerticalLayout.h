#pragma once
#include <TML/Interface/BaseComponent.h>
#include <TML/Interface/Layout.h>

namespace tml::Interface
{
    class TML_API VerticalLayout : public Layout
    {
    public:
        VerticalLayout() noexcept;
        VerticalLayout(int32_t x, int32_t y, uint32_t w, uint32_t h) noexcept;
        explicit VerticalLayout(const std::vector<BaseComponent*>& components,
                                int32_t x = 0, int32_t y = 0, uint32_t h = 0) noexcept;

    private:
        void ScaleChildren() noexcept final;
        void AlignChildren() noexcept final;
    };
}