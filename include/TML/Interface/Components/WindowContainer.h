///
/// Component that fills the whole window, you can then add a layout to this container.
///

#pragma once
#include <TML/Interface/BaseComponent.h>
#include <TML/Interface/Layout.h>

namespace tml
{
    namespace Interface
    {
        class WindowContainer : public BaseComponent
        {
        public:
            WindowContainer(RenderWindow& window);
        private:
            void Draw(RenderWindow& renderWindow) override;
        };
    }
}