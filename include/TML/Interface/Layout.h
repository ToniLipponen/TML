#pragma once
#include <TML/Vector2.h>
#include <TML/Interface/InterfaceComponent.h>
#include <vector>

namespace tml::Interface
{
    class Layout : public Object
    {
    public:
        using Object::Object;
        void Update();
        void AddComponent(BaseComponent* component);
        void ForEachComponent(Function_ptr<void, BaseComponent*> func);
    private:
        virtual void UpdateComponents() = 0;
    protected:
        std::vector<BaseComponent*> m_components;
    };
}