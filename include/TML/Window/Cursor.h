#pragma once
#include <TML/System/Math/Vector2.h>
#include <TML/System/Image.h>
#include <TML/Export.h>

namespace tml
{
    class TML_API Cursor
    {
    private:
        explicit Cursor(int shape) noexcept;

    public:
        Cursor() noexcept;
        Cursor(const Cursor&) = delete;
        explicit Cursor(const Image& image, int pointX = 0, int pointY = 0) noexcept;
        virtual ~Cursor() noexcept;

        Cursor& operator=(const Cursor& rhs) = delete;

        bool Create(const Image& image, int pointX = 0, int pointY = 0) noexcept;
        void* GetHandle() const noexcept;

        [[maybe_unused]] const static Cursor CrosshairCursor;
        [[maybe_unused]] const static Cursor IBeamCursor;
        [[maybe_unused]] const static Cursor ArrowCursor;
        [[maybe_unused]] const static Cursor VResizeCursor;
        [[maybe_unused]] const static Cursor HResizeCursor;
        [[maybe_unused]] const static Cursor CenterCursor;
        [[maybe_unused]] const static Cursor HandCursor;
        [[maybe_unused]] const static Cursor DefaultCursor;

    private:
        void* m_handle = nullptr;
    };
}