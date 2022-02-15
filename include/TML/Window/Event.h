#pragma once
#include "TML/Types.h"
#include <queue>

namespace tml
{
    class Event
    {
    public:
        struct ResizeEvent
        {
            i32 x;
            i32 y;
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

        struct MouseMoveEvent
        {
            i32 x; //!< X position of the mouse pointer, relative to the left of the owner window
            i32 y; //!< Y position of the mouse pointer, relative to the top of the owner window
        };

        struct MouseButtonEvent
        {
            ui32 button; //!< Code of the button that has been pressed
            i32 x;       //!< X position of the mouse pointer, relative to the left of the owner window
            i32 y;       //!< Y position of the mouse pointer, relative to the top of the owner window
        };

        struct MouseWheelScrollEvent
        {
            float delta; //!< Wheel offset (positive is up/left, negative is down/right). High-precision mice may use non-integral offsets.
            i32 x;     //!< X position of the mouse pointer, relative to the left of the owner window
            i32 y;     //!< Y position of the mouse pointer, relative to the top of the owner window
        };

        struct UpdateEvent
        {
            double delta;
        };

        struct DragAndDropEvent
        {
            i32 count;      //!< Number of dropped files
            char** paths;   //!< File paths in UTF-8 strings (memory needs to be freed manually)
        };

        enum EventType
        {
            Null,
            Closed,                 //!< The window requested to be closed (no data)
            WindowResized,          //!< The window was resized (data in event.size)
            InterfaceResized,       //!< tml::Interface component is resized (data in event.size)
            InterfaceMoved,         //!< tml::Interface component is moved (data in event.move)
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
            InterfaceUpdate,        //!< tml::Interface component update event. (data in event.update)
            FileDragAndDropped,     //!< File or files dragged onto a window. (data in event.dragAndDrop)

        };

        EventType type = EventType::Null;
        void* sender = nullptr;

        union
        {
            MoveEvent             move;
            ResizeEvent           size;
            KeyEvent              key;
            UpdateEvent           update;
            TextEvent             text;
            MouseMoveEvent        mouseMove;
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

        Event PollEvents() noexcept;
        Event WaitEvents() noexcept;
        void PushEvent(Event& event);   /// Pushed new event to the event queue.
        Event PopEvent() noexcept;      /// Get event from event queue. Might return Null event. Event::EventType::Null
    protected:
        void PollMouse();
    private:
        std::queue<Event> m_eventQueue;
        static EventSystem* m_instance;
    };
}