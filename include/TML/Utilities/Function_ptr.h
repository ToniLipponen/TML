#pragma once

namespace tml
{
    template<typename return_value, typename ... arguments>
    class Function_ptr
    {
    public:
        Function_ptr() = default;
        constexpr Function_ptr(return_value (*func_ptr)(arguments ...))
        : m_funcPtr(func_ptr)
        {

        }
        inline return_value operator()(arguments ... b) const noexcept
        {
            return m_funcPtr(b ...);
        }
        inline constexpr bool IsNull() const noexcept { return (m_funcPtr == nullptr); }
        inline constexpr bool IsNotNull() const noexcept { return (m_funcPtr != nullptr); }
    private:
        return_value (*m_funcPtr)(arguments ...) = nullptr;
    };
}