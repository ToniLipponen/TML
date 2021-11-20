![Logo](https://user-images.githubusercontent.com/86045205/139578779-10c9f5cc-1185-4a45-a500-09b9c02be1ff.png)
  
# TML
TML is a simple library for making games and applications. It provides relatively simple interfaces for window creation, graphics rendering, audio playback and keyboard/mouse input. 

# Building
<h2>Linux:</h2>
Before building on Linux, you might need to install the X11 development packages.<br>
On Ubuntu base distributions: <code>sudo apt install xorg-dev</code><br>
On RHEL/Fedora based distributions:
<code>sudo dnf install libXcursor-devel libXi-devel libXinerama-devel libXrandr-devel</code><br><br>
And then in terminal:<br>
1. <code>git clone --recursive "https://github.com/ToniLipponen/TML.git"<br>
2. cd TML<br>
3. cmake .<br>
4. make -j8</code><br>

<h2>Windows (Visual Studio):</h2>
In CMD:<br>
1. <code>git clone --recursive "https://github.com/ToniLipponen/TML.git"<br>
2. cd TML<br>
3. cmake -G "Visual Studio 17"</code><br>
4. Open the generated .sln in Visual Studio<br>
5. Build.

# Using
Currently there are no API documentation or tutorials on how to use this library. There are some VERY simple examples in the <b>examples</b> directory. Once this project is a bit more mature, I will take the time to document the API and make some tutorials.

# Contributing
If you're interested in this project, you can help us in a couple different ways. 
1. Contribute code to the library.
2. Report issues.
3. Create feature requests.
4. Create example applications.
5. Testing.

# Credits
|Author|Repo|Library|License|
|------|----|---|-------|
|Sean Barret|https://github.com/nothings/stb|stb libraries|Public Domain|
|David Herberth|https://github.com/Dav1dde/glad|glad|MIT|
|glfw|https://github.com/glfw/glfw|glfw|Zlib|
|Christophe|https://github.com/Groovounet/glm|glm|MIT|
|Dale Weiler|https://github.com/graphitemaster/incbin|incbin|Unlicense License|
|[Mentioned in 'external/libwebp/AUTHORS']|https://github.com/webmproject/libwebp/|libwebp|BSD 3-clause License|
|David Reid|https://github.com/mackron/miniaudio|miniaudio|Public Domain|
|Dominic Szablewski|https://github.com/phoboslab/pl_mpeg|pl_mpeg|MIT|
