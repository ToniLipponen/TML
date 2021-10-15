#include <TML/Interface/Container.h>

namespace tml::Interface
{
    void Container::AddComponent(BaseComponent *component)
    {
        if(m_layout)
            m_layout->AddComponent(component);
    }
}