# Building

## Prerequisites Linux

Before building on Linux, you need to install the X11 development packages, git, gcc and cmake.

#### Ubuntu/Debian:

<code>sudo apt install gcc xorg-dev git cmake </code>

#### RHEL/Fedora:

<code>sudo dnf install gcc libXcursor-devel libXi-devel libXinerama-devel libXrandr-devel git cmake </code>

#### Arch:

<code>sudo pacman -S gcc libx11 git cmake </code>

## Prerequisites FreeBSD

Before building on FreeBSD.<br>
<code>sudo pkg install gcc cmake git pkgconf xorg</code>

## Prerequisites Windows

1. Download and install cmake from https://cmake.org
2. I guess you just install Visual Studio. Or get a compiler some other way.

## Build

In the terminal / CMD:<br>

1. <code>git clone --recursive "https://github.com/ToniLipponen/TML.git" </code>
2. <code>cd TML</code>
3. <code>cmake -B build</code>
4. <code>cmake --build build --config Release</code>

This should build the project into the *build* directory. In the build directory there is a folder called lib, which contains the library files.
