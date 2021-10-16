#include <TML/Interface/Layout.h>

namespace tml::Interface
{
    void Layout::AddComponent(BaseComponent *component)
    {
        this->AddChild(component);
        UpdateComponents();
    }

    void Layout::ForEachComponent(Function_ptr<void, BaseComponent *> func)
    {
        for(auto i : m_children)
            func(i.second);
    }

    void Layout::Update(float dt)
    {
        UpdateComponents();
        BaseComponent::Update(dt);
    }
}