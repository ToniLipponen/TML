/** This file is partially based on Laurent Gomilas sf::Event. Specifically the Event class. */

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
#include "TML/Types.h"
#include <queue>
#include <map>

namespace tml
{
    struct Event
    {
        struct ResizeEvent
        {
            i32 w;
            i32 h;
        };

        struct MoveEvent
        {
            i32 x;
            i32 y;
        };

        struct KeyEvent
        {
            i32 code;     //!< Code of the key that has been pressed
            bool alt;     //!< Is the Alt key pressed?
            bool control; //!< Is the Control key pressed?
            bool shift;   //!< Is the Shift key pressed?
            bool system;  //!< Is the System key pressed?
        };

        struct TextEvent
        {
            ui32 unicode; //!< UTF-32 Unicode value of the character
        };

        struct MouseButtonEvent
        {
            ui32 button; //!< Code of the button that has been pressed
            i32 x;       //!< X position of the mouse pointer, relative to the left of the owner window
            i32 y;       //!< Y position of the mouse pointer, relative to the top of the owner window
        };

        struct MouseWheelScrollEvent
        {
            float delta; //!< Vertical scroll offset.
            i32 x;       //!< X position of the mouse pointer, relative to the left of the owner window
            i32 y;       //!< Y position of the mouse pointer, relative to the top of the owner window
        };

        struct DragAndDropEvent
        {
            i32 count;      //!< Number of dropped files
            char** paths;   //!< File paths in UTF-8 strings (memory needs to be freed manually)
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
            InterfaceResized,       //!< tml::Interface component is resized (data in event.size)
            InterfaceMoved,         //!< tml::Interface component is moved (data in event.pos)
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
            DragAndDrop,            //!< File or files dragged onto a window. (data in event.dragAndDrop)
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
            DragAndDropEvent      dragAndDrop;
        };
    };

    class EventSystem
    {
    private:
        EventSystem();
    public:
        EventSystem(EventSystem&)           = delete;
        EventSystem(EventSystem&&)          = delete;
        void operator=(const EventSystem&)  = delete;
        static EventSystem& GetInstance();

        bool Register(const void* handle) noexcept;          //!< Register new sender / emitter.
        bool Remove(const void* handle) noexcept;            //!< Remove a sender / emitter.
        bool PollEvents(const void*, Event& e) noexcept;     //!< Checks if there is events in the event queue.
        bool WaitEvents(const void*, Event& e) noexcept;     //!< Blocks until there is an event in the event queue.
        void PushEvent(const void*, Event& event) noexcept;  //!< Pushed new event to the event queue.
        bool PopEvent(const void*, Event& e) noexcept;       //!< Get event from event queue. Returns true if popped new event.

    private:
        std::map<const void*, std::queue<Event>> m_handles;
        static EventSystem* m_instance;
    };
}