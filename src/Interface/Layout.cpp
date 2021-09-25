#include <TML/Interface/Layout.h>

namespace tml::Interface
{
    Layout::Layout(const Vector2 &position, const Vector2 &size)
    {
        m_pos = position;
        m_size = size;
    }

    void Layout::AddComponent(BaseComponent *component)
    {
        m_components.push_back(component);
        UpdateComponents();
    }

    void Layout::Update()
    {
        UpdateComponents();
        for(auto i : m_components)
            i->Update();
    }
}