#pragma once
#include <TML/Vector2.h>
#include <TML/Interface/InterfaceComponent.h>
#include <vector>

namespace tml::Interface
{
    class Layout : public Object
    {
    public:
        Layout(const Vector2& position, const Vector2& size);
        void Update();
        void AddComponent(BaseComponent* component);
    private:
        virtual void UpdateComponents() = 0;
    protected:
        std::vector<BaseComponent*> m_components;
    };
}