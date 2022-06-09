#include <TML/Interface/Components/Spacer.h>

namespace tml::Interface
{
    Spacer::Spacer() noexcept
    : BaseComponent()
    {
        m_vSizePolicy = SizePolicy::Expand;
        m_hSizePolicy = SizePolicy::Expand;
    }
}