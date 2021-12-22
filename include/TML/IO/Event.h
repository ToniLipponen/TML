#pragma once
#include <TML/Utilities/Types.h>
#include <queue>

namespace tml
{
    class Event
    {
    public:
        struct ResizeEvent
        {
            ui32 width;
            ui32 height;
        };

        struct KeyEvent
        {
            i32 code;    //!< Code of the key that has been pressed
            bool alt;     //!< Is the Alt key pressed?
            bool control; //!< Is the Control key pressed?
            bool shift;   //!< Is the Shift key pressed?
            bool system;  //!< Is the System key pressed?
        };

        struct UpdateEvent
        {
            double delta;
        };

        struct TextEvent
        {
            ui32 unicode; //!< UTF-32 Unicode value of the character
        };

        struct MouseMoveEvent
        {
            int x; //!< X position of the mouse pointer, relative to the left of the owner window
            int y; //!< Y position of the mouse pointer, relative to the top of the owner window
        };

        struct MouseButtonEvent
        {
            ui32 button; //!< Code of the button that has been pressed
            int x;       //!< X position of the mouse pointer, relative to the left of the owner window
            int y;       //!< Y position of the mouse pointer, relative to the top of the owner window
        };

        struct MouseWheelScrollEvent
        {
            float        delta; //!< Wheel offset (positive is up/left, negative is down/right). High-precision mice may use non-integral offsets.
            int          x;     //!< X position of the mouse pointer, relative to the left of the owner window
            int          y;     //!< Y position of the mouse pointer, relative to the top of the owner window
        };

        enum EventType
        {
            Null,
            Closed,                 //!< The window requested to be closed (no data)
            Resized,                //!< The window was resized (data in event.size)
            LostFocus,              //!< The window lost the focus (no data)
            GainedFocus,            //!< The window gained the focus (no data)
            TextEntered,            //!< A character was entered (data in event.text)
            KeyPressed,             //!< A key was pressed (data in event.key)
            KeyReleased,            //!< A key was released (data in event.key)
            MouseWheelScrolled,     //!< The mouse wheel was scrolled (data in event.mouseWheelScroll)
            MouseButtonPressed,     //!< A mouse button was pressed (data in event.mouseButton)
            MouseButtonReleased,    //!< A mouse button was released (data in event.mouseButton)
            MouseMoved,             //!< The mouse cursor moved (data in event.mouseMove)
            MouseEntered,           //!< The mouse cursor entered the area of the window (no data)
            MouseLeft,              //!< The mouse cursor left the area of the window (no data)
        };

        enum EventCategory
        {
            None, Window, Keyboard, Mouse
        };

        EventCategory category = EventCategory::None;
        EventType type = EventType::Null;

        void* sender = nullptr;

        union
        {
            ResizeEvent           size;
            KeyEvent              key;
            UpdateEvent           update; // Not used
            TextEvent             text;
            MouseMoveEvent        mouseMove;
            MouseButtonEvent      mouseButton;
            MouseWheelScrollEvent mouseWheelScroll;
        };
    };

    class EventSystem
    {
    private:
        EventSystem();
    public:
        const static ui64 AnyID = 0;
    public:
        EventSystem(EventSystem&)           = delete;
        EventSystem(EventSystem&&)          = delete;
        void operator=(const EventSystem&)  = delete;
        static EventSystem& GetInstance();

        Event PollEvents() noexcept;
        Event WaitEvents() noexcept;
        void PushEvent(Event& event); // Pushed new event to the event queue.
        Event PopEvent(bool block = false) noexcept; // Get event from event queue. Might return Null event. Event::EventType::Null

    protected:
        void PollMouse() noexcept;
    private:
        std::queue<Event> m_eventQueue;
        ui64 m_counter = 0;
        static EventSystem* m_instance;
    };
}