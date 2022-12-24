#pragma once
#include <vector>
#include <map>
#include <TML/System/String.h>

namespace tml
{
    class DropManager
    {
    private:
        DropManager() noexcept;

    public:
        static DropManager& GetInstance() noexcept;
        void Set(const void* handle, std::vector<String>& paths) noexcept;
        std::vector<String> Get(const void* handle) noexcept;

    private:
        std::map<const void*, std::vector<String>> m_paths;
    };
}