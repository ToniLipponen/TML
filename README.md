![Logo](https://user-images.githubusercontent.com/86045205/139578779-10c9f5cc-1185-4a45-a500-09b9c02be1ff.png)

[![License: Zlib](https://img.shields.io/badge/License-Zlib-red.svg)](https://opensource.org/licenses/Zlib)
[![Build on linux](https://github.com/ToniLipponen/TML/actions/workflows/build_linux.yml/badge.svg?branch=master)](https://github.com/ToniLipponen/TML/actions/workflows/build_linux.yml)
[![Build on windows](https://github.com/ToniLipponen/TML/actions/workflows/build_windows.yml/badge.svg?branch=master)](https://github.com/ToniLipponen/TML/actions/workflows/build_windows.yml)

TML is a simple library for making games and applications. It provides interfaces for window creation, graphics rendering, audio playback and keyboard/mouse/controller input. 

# Notable features
- 2D batch renderer, including sprite batching
- Compute shaders through OpenGL 4.5
- No external dependencies
- Modular design
- Written in C++14
- OpenGL 4.5 and OpenGL ES 3.1 backends

# Modules
- Audio
- Graphics
- Network
- System
- Window

# Supported platforms
- <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/0/0a/Unofficial_Windows_logo_variant_-_2002–2012_%28Multicolored%29.svg/1161px-Unofficial_Windows_logo_variant_-_2002–2012_%28Multicolored%29.svg.png" width=14 height=14 alt=""> Windows
- <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/3/35/Tux.svg/1200px-Tux.svg.png" width=14 height=14 alt=""> Linux
- <img src="https://seeklogo.com/images/F/freebsd-logo-542DF4765A-seeklogo.com.png" width=14 height=14 alt=""> FreeBSD

Currently, I have no way of supporting macOS.

# Building
If you wish to build the project from source, there are some instructions in [BUILDING.md](BUILDING.md)

# Using
Currently, there is no API documentation or tutorials on how to use this library. There are some very simple examples in the [examples](examples) directory. Once this project is a bit more matured, I will take the time to document the API and make some tutorials.

### Basic example
    
```cpp
#include <TML/Graphics.h>
using namespace tml;

int main()
{
    RenderWindow window(800, 600, "Circle");

    while(window.IsOpen())
    {
        Event event{};

        while(window.PollEvents(event))
        {
            if(event.type == Event::Closed)
                window.Close();
        }

        window.Clear();
        window.DrawCircle(window.GetSize() / 2, 200, Color::Red);
        window.Display();
    }
    return 0;
}
```

# Contributing
If you're interested in this project, you can help by testing, contributing code, reporting bugs or other issues, and suggesting new features or changes.

# License
This project is licensed under the [Zlib License](LICENSE). External libraries used by TML are also distributed under very permissive licenses (See below). This means you don't have to worry about licensing, when developing software using TML.

# Credits
|Author| Repo                                 |Library|License|
|------|--------------------------------------|---|-------|
|Sean Barret| https://github.com/nothings/stb      |stb libraries|Public Domain|
|David Herberth| https://github.com/Dav1dde/glad      |glad|MIT|
|glfw| https://github.com/glfw/glfw         |glfw|Zlib|
|G-Truc| https://github.com/g-truc/glm        |glm|MIT|
| David Reid                             | https://github.com/mackron/miniaudio |miniaudio|Public Domain|
| David Capello                          | https://github.com/dacap/clip        |clip|MIT|
| Nwutobo Samuel Ugochukwu               | https://github.com/sammycage/lunasvg |lunasvg|MIT|

### And a special thank you to JetBrains for providing me with a free license for CLion.
# Contact
You can contact me by email.

toni.lipponen@protonmail.com
