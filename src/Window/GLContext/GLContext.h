#pragma once

namespace tml
{
    class GLContext
    {
    private:
        GLContext();
    public:
        static GLContext& GetInstance() noexcept;
        void* GetContextHandle() const noexcept;
        
    private:
        void* m_contextHandle = nullptr;
        static GLContext* s_glContext;
    };
}