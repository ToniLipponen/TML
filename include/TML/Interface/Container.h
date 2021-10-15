#pragma once
#include <TML/Interface/InterfaceComponent.h>
#include <TML/Interface/Layout.h>

namespace tml
{
    namespace Interface
    {
        class Container : public BaseComponent
        {
        public:
            void AddChild(BaseComponent*) = delete;
            void AddComponent(BaseComponent* component);
        protected:
            void Draw() override {}
//            void OnUpdate(float dt) override;
            Layout* m_layout;
        };
    }
}