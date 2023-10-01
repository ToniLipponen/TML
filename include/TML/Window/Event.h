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
        int32_t w;
        int32_t h;
    };

    struct MoveEvent : Event
    {
        int32_t x;
        int32_t y;
    };

    struct KeyEvent : Event
    {
        int32_t code;         //!< Code of the key that has been pressed
        Keyboard::Key value;  //!< The key in question.
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
        int32_t x;            //!< X position of the mouse pointer, relative to the left of the owner window
        int32_t y;            //!< Y position of the mouse pointer, relative to the top of the owner window
    };

    struct MouseScrollEvent : Event
    {
        float deltaX;    //!< Horizontal scroll offset.
        float deltaY;    //!< Vertical scroll offset.
        int32_t x;       //!< X position of the mouse pointer, relative to the left of the owner window
        int32_t y;       //!< Y position of the mouse pointer, relative to the top of the owner window
    };

    struct MouseDragEvent : Event
    {
        Mouse::Button button;     //!< Which button was used to drag.
        int32_t x;                //!< Current X position of the cursor.
        int32_t y;                //!< Current Y position of the cursor.
        int32_t beginX;           //!< Begin X position of the drag event.
        int32_t beginY;           //!< Begin Y position of the drag event.
    };

    struct GamepadEvent : Event
    {
        bool connected; //!< Connected/disconnected
        int32_t id;     //!< Gamepad id
    };

    struct FileDropEvent : Event
    {
        std::vector<String> files;
    };
}