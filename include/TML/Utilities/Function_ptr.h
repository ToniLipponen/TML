// Small abstraction layer for function pointers.
#pragma once

namespace tml
{
    template<typename return_value, typename ... arguments>
    class Function_ptr
    {
    public:
        Function_ptr(){}
        constexpr Function_ptr(return_value (*func_ptr)(arguments ...))
        : m_funcPtr(func_ptr)
        {

        }
        return_value operator()(arguments ... b) const noexcept
        {
            return m_funcPtr(b ...);
        }
        bool IsNull() const { return (m_funcPtr == nullptr); }
        bool IsNotNull() const { return (m_funcPtr != nullptr); }
    private:
        return_value (*m_funcPtr)(arguments ...) = nullptr;
    };
}