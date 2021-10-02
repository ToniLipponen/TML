#include <TML/Interface/Layout.h>

namespace tml::Interface
{
    void Layout::AddComponent(BaseComponent *component)
    {
        m_components.push_back(component);
        UpdateComponents();
    }

    void Layout::ForEachComponent(Function_ptr<void, BaseComponent *> func)
    {
        for(auto i : m_components)
            func(i);
    }

    void Layout::Update()
    {
        UpdateComponents();
        for(auto i : m_components)
            i->Update();
    }
}