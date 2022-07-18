#include <TML/Interface/Components/Spacer.h>

namespace tml::Interface
{
    Spacer::Spacer() noexcept
    : Component()
    {
        m_vSizePolicy = SizePolicy::Expand;
        m_hSizePolicy = SizePolicy::Expand;
    }

    Spacer::Spacer(ComponentAxis axis, int size) noexcept
    : Component()
    {
        if(axis == ComponentAxis::Horizontal)
        {
            m_size.x = size;
            m_vSizePolicy = SizePolicy::Expand;
            m_hSizePolicy = SizePolicy::Fixed;
        }
        else
        {
            m_size.y = size;
            m_vSizePolicy = SizePolicy::Fixed;
            m_hSizePolicy = SizePolicy::Expand;
        }
    }
}