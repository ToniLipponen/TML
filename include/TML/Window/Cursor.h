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
        [[nodiscard]] void* GetHandle() const noexcept;
        
        const static Cursor CrosshairCursor;
        const static Cursor IBeamCursor;
        const static Cursor ArrowCursor;
        const static Cursor VResizeCursor;
        const static Cursor HResizeCursor;
        const static Cursor CenterCursor;
        const static Cursor HandCursor;
        const static Cursor DefaultCursor;

    private:
        void* m_handle = nullptr;
    };
}