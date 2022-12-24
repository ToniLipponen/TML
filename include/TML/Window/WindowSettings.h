#pragma once
#include <TML/System/Math/Vector2.h>
#include <TML/System/Image.h>
#include <TML/Window/Monitor.h>
#include <cstdint>
#include <optional>

namespace tml
{
    struct WindowSettings
    {
        enum Flag : int32_t
        {
            Fullscreen           = (1 <<  0), //!< Make the window fullscreen. If you want to use the monitors resolution, use the UseMonitorResolution setting.
            Resizeable           = (1 <<  1), //!< Make the window resizeable.
            Maximized            = (1 <<  2), //!< Make the window maximized.
            VSync                = (1 <<  3), //!< Use vertical synchronization.
            Antialias            = (1 <<  4), //!< Enable antialiasing.
            NoTopBar             = (1 <<  5), //!< Make a window with no top bar / title bar.
            LimitAspect          = (1 <<  6), //!< Limit the window aspect ratio to the aspect ratio of width & height.
            Hidden               = (1 <<  7), //!< Make the window hidden.
            Transparent          = (1 <<  8), //!< Make the window transparent. (Note: Might not be supported on all platforms).
            AlwaysOnTop          = (1 <<  9), //!< Keep the window above other windows.
            UseMonitorResolution = (1 << 10), //!< Use the primary monitors size instead of user supplied width & height.
            NoClient             = (1 << 11), //!< Dont create an OpenGL context.
            Minimized            = (1 << 12), //!< Make the window minimized.
        };

        Vector2i size;
        uint32_t framerate;
        int32_t antialiasSamples;
        std::optional<Vector2i> position;
        std::optional<Vector2i> minimumSize;
        std::optional<Vector2i> maximumSize;
        std::optional<Monitor> monitor;
        std::optional<Image> icon;
        String title;
        int32_t flags;
    };
}