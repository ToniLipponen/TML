#pragma once
#include <TML/Export.h>

namespace tml
{
    class TML_API GLContext
    {
    private:
        GLContext();
        ~GLContext();

    public:
        static GLContext& GetInstance() noexcept;
        void* GetContextHandle() const noexcept;
        
    private:
        void* m_contextHandle = nullptr;
    };
}