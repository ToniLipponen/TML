![Logo](https://user-images.githubusercontent.com/86045205/139578779-10c9f5cc-1185-4a45-a500-09b9c02be1ff.png)

[![License: Zlib](https://img.shields.io/badge/License-Zlib-red.svg)](https://opensource.org/licenses/Zlib)


[![Build on linux](https://github.com/ToniLipponen/TML/actions/workflows/build_linux.yml/badge.svg?branch=master)](https://github.com/ToniLipponen/TML/actions/workflows/build_linux.yml)
[![Build on windows](https://github.com/ToniLipponen/TML/actions/workflows/build_windows.yml/badge.svg?branch=master)](https://github.com/ToniLipponen/TML/actions/workflows/build_windows.yml)

TML is a simple library for making games and applications. It provides simple interfaces for window creation, graphics rendering, audio playback and keyboard/mouse input. 

# Supported platforms
- <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/0/0a/Unofficial_Windows_logo_variant_-_2002–2012_%28Multicolored%29.svg/1161px-Unofficial_Windows_logo_variant_-_2002–2012_%28Multicolored%29.svg.png" width=14 height=14> Windows
- <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/3/35/Tux.svg/1200px-Tux.svg.png" width=14 height=14> Linux
- <img src="https://seeklogo.com/images/F/freebsd-logo-542DF4765A-seeklogo.com.png" width=14 height=14> FreeBSD


Currently I have no way of supporting MacOS.

# Building

<h2>Prerequisites Linux</h2>
Before building on Linux, you need to install the X11 development packages, git and cmake.<br><br>
Ubuntu/Debian: <code>sudo apt install xorg-dev git cmake</code><br>
RHEL/Fedora: <code>sudo dnf install libXcursor-devel libXi-devel libXinerama-devel libXrandr-devel git cmake</code><br>
Arch: <code>sudo pacman -S libx11 git cmake</code>

<h2>Prerequisites FreeBSD</h2>
Before building on FreeBSD.<br>
<code>sudo pkg install gcc cmake git pkgconf xorg </code><br><br>

<h2>Prerequisites Windows</h2>
1. Download and install cmake from https://cmake.org <br>
2. I guess you just install Visual Studio. Or get a compiler some other way.<br><br>

<h2>Build</h2>
In the terminal / CMD:<br>
1. <code>git clone --recursive "https://github.com/ToniLipponen/TML.git" </code><br>
2. <code>cd TML</code><br>
3. <code>cmake -B build</code><br>
4. <code>cmake --build build</code><br>

This should build the project into the <b>build</b> directory. In the build directory there is a folder called lib, which contains the library files. And in the examples folder there are some small example programs.

# Using
Currently, there are no API documentation or tutorials on how to use this library. There are some VERY simple examples in the <b>examples</b> directory. Once this project is a bit more matured, I will take the time to document the API and make some tutorials.

# Contributing
If you're interested in this project, you can help in a couple of different ways:

1. Contribute code to the library.
2. Review code.
3. Report bugs and other issues.
4. Create feature requests.
5. Create example applications.
6. Testing.

# Credits
|Author|Repo|Library|License|
|------|----|---|-------|
|Sean Barret|https://github.com/nothings/stb|stb libraries|Public Domain|
|David Herberth|https://github.com/Dav1dde/glad|glad|MIT|
|glfw|https://github.com/glfw/glfw|glfw|Zlib|
|G-Truc|https://github.com/g-truc/glm|glm|MIT|
|[Mentioned in 'external/libwebp/AUTHORS']|https://github.com/webmproject/libwebp/|libwebp|BSD 3-clause License|
| David Reid                             |https://github.com/mackron/miniaudio|miniaudio|Public Domain|
| Dominic Szablewski                     |https://github.com/phoboslab/pl_mpeg|pl_mpeg|MIT|
| David Capello                          |https://github.com/dacap/clip|clip|MIT|
| Nwutobo Samuel Ugochukwu               |https://github.com/sammycage|lunasvg|MIT|
