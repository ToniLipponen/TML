#pragma once
#include <TML/Interface/BaseComponent.h>
#include <TML/Interface/Layouts/HorizontalLayout.h>

namespace tml
{
    namespace Interface
    {
        class Menubar : public HorizontalLayout
        {
        public:
            explicit Menubar(RenderWindow& window);
        private:
            void Draw(RenderWindow& renderWindow) override;
        };
    }
}