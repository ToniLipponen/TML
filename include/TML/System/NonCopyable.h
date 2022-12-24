#pragma once

namespace tml
{
    struct NonCopyable
    {
        NonCopyable() noexcept = default;
        NonCopyable(const NonCopyable&) = delete;
        NonCopyable(NonCopyable&&) noexcept = delete;

        NonCopyable& operator=(const NonCopyable&) = delete;
        NonCopyable& operator=(NonCopyable&&) noexcept = delete;
    };
}