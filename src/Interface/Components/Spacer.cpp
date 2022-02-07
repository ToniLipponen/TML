#include <TML/Interface/Components/Spacer.h>

namespace tml
{
    namespace Interface
    {
        Spacer::Spacer()
        : BaseComponent()
        {
            m_vSizePolicy = Expand;
            m_hSizePolicy = Expand;
        }
    }
}