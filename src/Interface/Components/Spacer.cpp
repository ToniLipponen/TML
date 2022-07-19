#include <TML/Interface/Components/Spacer.h>

namespace tml::Interface
{
    Spacer::Spacer() noexcept
    : Component()
    {
        m_vSizePolicy = SizePolicy::Dynamic;
        m_hSizePolicy = SizePolicy::Dynamic;
    }

    Spacer::Spacer(ComponentAxis axis, int min, int max) noexcept
    : Component()
    {
        m_vSizePolicy = SizePolicy::Dynamic;
        m_hSizePolicy = SizePolicy::Dynamic;

        if(axis == ComponentAxis::Horizontal)
        {
            m_minimumSize.x = m_size.x = min;
            m_maximumSize.x  = max;
        }
        else
        {
            m_minimumSize.y = m_size.y = min;
            m_maximumSize.y  = max;
        }
    }
}