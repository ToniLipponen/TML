#pragma once

namespace tml
{
    template<typename return_value, typename ... arguments>
    class Function_ptr
    {
    public:
        Function_ptr(return_value (*func_ptr)(arguments ...))
                : m_func_ptr(func_ptr)
        {

        }
        return_value operator()(arguments ... b)
        {
            return m_func_ptr(b ...);
        }

    private:
        return_value (*m_func_ptr)(arguments ...);
    };
}