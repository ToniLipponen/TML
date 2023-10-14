#pragma once
#include <TML/Export.h>
#include <TML/Window/Input.h>
#include <TML/System/String.h>
#include <TML/System/EventHandler.h>
#include <cstdint>
#include <vector>

namespace tml
{
    struct ResizeEvent : Event
    {
        Vector2i size;
    };

    struct MoveEvent : Event
    {
        Vector2i position;
    };

    struct KeyEvent : Event
    {
        int32_t code;         //!< Code of the key that has been pressed
        Keyboard::Key key;    //!< The key in question.
        bool alt;             //!< Is the Alt key pressed?
        bool control;         //!< Is the Control key pressed?
        bool shift;           //!< Is the Shift key pressed?
        bool system;          //!< Is the System key pressed?
    };

    struct TextEvent : Event
    {
        uint32_t unicode; //!< UTF-32 Unicode value of the character
    };

    struct MouseButtonEvent : Event
    {
        Mouse::Button button; //!< Which ever button was pressed.
        Vector2i position;
    };

    struct MouseScrollEvent : Event
    {
        Vector2i position;  //!< Position of the mouse pointer
        Vector2f delta;     //!< Scroll offset
    };

    struct FileDropEvent : Event
    {
        std::vector<String> files;
    };
}