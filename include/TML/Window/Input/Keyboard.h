#pragma once
#include <TML/Export.h>

namespace tml::Keyboard
{
    enum Key
    {
        Space = 32,
        Apostrophe = 39,  /* ' */
        Comma = 44,  /* , */
        Minus = 45,  /* - */
        Period = 46,  /* . */
        Slash = 47,  /* / */
        Zero = 48,
        One = 49,
        Two = 50,
        Three = 51,
        Four = 52,
        Five = 53,
        Six = 54,
        Seven = 55,
        Eight = 56,
        Nine = 57,
        Semicolon = 59,  /* ; */
        Equal = 61,  /* = */
        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,
        LeftBracket = 91,  /* [ */
        Backslash = 92,  /* \ */
        RightBracket = 93,  /* ] */
        GraveAccent = 96,  /* ` */
        World1 = 161, /* non-US #1 */
        World2 = 162, /* non-US #2 */

        Escape = 256,
        Esc = Escape,
        Enter = 257,
        Tab = 258,
        Backspace = 259,
        Insert = 260,
        Delete = 261,
        RightArrow = 262,
        LeftArrow = 263,
        DownArrow = 264,
        UpArrow = 265,
        PageUp = 266,
        PageDown = 267,
        Home = 268,
        End = 269,
        CapsLock = 280,
        ScrollLock = 281,
        NumLock = 282,
        PrintScreen = 283,
        Pause = 284,
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301,
        F13 = 302,
        F14 = 303,
        F15 = 304,
        F16 = 305,
        F17 = 306,
        F18 = 307,
        F19 = 308,
        F20 = 309,
        F21 = 310,
        F22 = 311,
        F23 = 312,
        F24 = 313,
        F25 = 314,
        Keypad0 = 320,
        Keypad1 = 321,
        Keypad2 = 322,
        Keypad3 = 323,
        Keypad4 = 324,
        Keypad5 = 325,
        Keypad6 = 326,
        Keypad7 = 327,
        Keypad8 = 328,
        Keypad9 = 329,
        KeypadDecimal = 330,
        KeypadDivide = 331,
        KeypadMultiply = 332,
        KeypadSubtract = 333,
        KeypadAdd = 334,
        KeypadEnter = 335,
        KeypadEqual = 336,
        LeftShift = 340,
        LeftControl = 341,
        LeftAlt = 342,
        LeftSuper = 343,
        RightShift = 344,
        RightControl = 345,
        RightAlt = 346,
        RightSuper = 347,
        Menu = 348
    };

    [[nodiscard]] TML_API bool IsKeyPressed(Key key) noexcept;
    [[nodiscard]] TML_API bool IsKeyDown(Key key) noexcept;
}