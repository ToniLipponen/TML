/** This file is partially based on Laurent Gomilas sf::Event. So here is his copyright & license. */

////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2022 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#pragma once
#include <cstdint>
#include <queue>
#include <map>

namespace tml
{
    struct Event
    {
        struct ResizeEvent
        {
            int32_t w;
            int32_t h;
        };

        struct MoveEvent
        {
            int32_t x;
            int32_t y;
        };

        struct KeyEvent
        {
            int32_t code;     //!< Code of the key that has been pressed
            int32_t value;    //!< Key value. tml::Keyboard::Key cast to an int.
            bool alt;         //!< Is the Alt key pressed?
            bool control;     //!< Is the Control key pressed?
            bool shift;       //!< Is the Shift key pressed?
            bool system;      //!< Is the System key pressed?
        };

        struct TextEvent
        {
            uint32_t unicode; //!< UTF-32 Unicode value of the character
        };

        struct MouseButtonEvent
        {
            uint32_t button; //!< Code of the button that has been pressed
            int32_t x;       //!< X position of the mouse pointer, relative to the left of the owner window
            int32_t y;       //!< Y position of the mouse pointer, relative to the top of the owner window
        };

        struct MouseWheelScrollEvent
        {
            float deltaX;    //!< Horizontal scroll offset.
            float deltaY;    //!< Vertical scroll offset.
            int32_t x;       //!< X position of the mouse pointer, relative to the left of the owner window
            int32_t y;       //!< Y position of the mouse pointer, relative to the top of the owner window
        };

        struct GamepadEvent
        {
            int32_t id;
        };

        enum EventType
        {
            NullEvent,
            Closed,                 //!< The window requested to be closed (no data)
            WindowResized,          //!< The window was resized (data in event.size)
            WindowMinimized,        //!< The window was minimized (no data)
            WindowMaximized,        //!< The window was maximized (no data)
            WindowRestored,         //!< The window was restored (no data)
            WindowMoved,            //!< The window was moved (data in event.pos)
            InterfaceResized,       //!< Not used right now.
            InterfaceMoved,         //!< Not used right now.
            LostFocus,              //!< The window lost the focus (no data)
            GainedFocus,            //!< The window gained the focus (no data)
            TextEntered,            //!< A character was entered (data in event.text)
            KeyPressed,             //!< A key was pressed (data in event.key)
            KeyReleased,            //!< A key was released (data in event.key)
            MouseWheelScrolled,     //!< The mouse wheel was scrolled (data in event.mouseWheelScroll)
            MouseButtonPressed,     //!< A mouse button was pressed (data in event.mouseButton)
            MouseButtonReleased,    //!< A mouse button was released (data in event.mouseButton)
            MouseMoved,             //!< The mouse cursor moved (data in event.pos)
            MouseEntered,           //!< The mouse cursor entered the area of the window (no data)
            MouseLeft,              //!< The mouse cursor left the area of the window (no data)
            GamepadConnected,       //!< Gamepad was connected. (data in event.gamepad)
            GamepadDisconnected,    //!< Gamepad was disconnected. (data in event.gamepad)
        };

        EventType type = NullEvent;

        union
        {
            MoveEvent             pos;
            ResizeEvent           size;
            KeyEvent              key;
            TextEvent             text;
            MouseButtonEvent      mouseButton;
            MouseWheelScrollEvent mouseWheelScroll;
            GamepadEvent          gamepad;
        };
    };

    class EventSystem
    {
    private:
        EventSystem();

    public:
        EventSystem(EventSystem&)                   = delete;
        EventSystem(EventSystem&&)                  = delete;
        EventSystem& operator=(const EventSystem&)  = delete;
        EventSystem& operator=(EventSystem&&)       = delete;
        static EventSystem& GetInstance();

        bool Register(const void* handle) noexcept;          //!< Register new sender / emitter.
        bool Remove(const void* handle) noexcept;            //!< Remove a sender / emitter.
        bool PollEvents(const void*, Event& e) noexcept;     //!< Checks if there is events in the event queue.
        bool WaitEvents(const void*, Event& e) noexcept;     //!< Blocks until there is an event in the event queue.
        void PushEvent(const void*, Event& event) noexcept;  //!< Pushed new event to the event queue.
        void PushGlobalEvent(Event& event) noexcept;         //!< Push an event to all event queues.
        bool PopEvent(const void*, Event& e) noexcept;       //!< Get event from event queue. Returns true if popped new event.

    private:
        std::map<const void*, std::queue<Event>> m_handles;
    };
}