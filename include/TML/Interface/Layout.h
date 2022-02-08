#pragma once
#include <TML/Utilities/Types.h>

namespace tml
{
    namespace Interface
    {
        class Layout
        {
        public:
            void SetPadding(const Vector2i& padding){ m_padding = padding; }
            Vector2i GetPadding() const noexcept { return m_padding; }
        protected:
            Vector2i m_padding = {5, 5};
        };
    }
}