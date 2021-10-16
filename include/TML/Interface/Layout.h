#pragma once
#include <TML/Vector2.h>
#include <TML/Interface/InterfaceComponent.h>
#include <vector>

namespace tml::Interface
{
    class Layout : public BaseComponent
    {
    public:
        using BaseComponent::BaseComponent;
        virtual void Update(float dt) override;
        void AddComponent(BaseComponent* component);
        void ForEachComponent(Function_ptr<void, BaseComponent*> func);
        void SetOnClick(UIFunc) = delete;
        void SetOnMouseDown(UIFunc) = delete;
        void SetOnHover(UIFunc) = delete;
        void SetOnEnter(UIFunc) = delete;
        void SetOnExit(UIFunc) = delete;
        void SetOnDrag(UIFunc) = delete;
        void SetOnUpdate(UIFunc) = delete;
        void SetOnFocusGained(UIFunc) = delete;
        void SetOnFocusLost(UIFunc) = delete;

    private:
        virtual void UpdateComponents() = 0;
    protected:
        SizePolicy m_hSizePolicy = Expand;
        SizePolicy m_vSizePolicy = Expand;
    };
}