// Small abstraction layer for function pointers.
#pragma once

namespace tml
{
    template<typename return_value, typename ... arguments>
    class Function_ptr
    {
    public:
        constexpr Function_ptr(return_value (*func_ptr)(arguments ...))
        : m_func_ptr(func_ptr)
        {

        }
        return_value operator()(arguments ... b) const noexcept
        {
            return m_func_ptr(b ...);
        }
        return_value (*)(args ...) GetRawPointer() const noexcept
        {
            return m_func_ptr;
        }
    private:
        return_value (*m_func_ptr)(arguments ...);
    };
}