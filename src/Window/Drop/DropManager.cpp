#include "DropManager.h"

namespace tml
{
    DropManager::DropManager() noexcept = default;

    DropManager& DropManager::GetInstance() noexcept
    {
        static DropManager instance;
        return instance;
    }

    void DropManager::Set(const void* handle, std::vector<String>& paths) noexcept
    {
        m_paths[handle] = std::move(paths);
    }

    std::vector<String> DropManager::Get(const void* handle) noexcept
    {
        if(m_paths.find(handle) != m_paths.end())
        {
            return m_paths[handle];
        }
        else
        {
            return {};
        }
    }
}