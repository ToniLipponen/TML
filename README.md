![Logo](https://user-images.githubusercontent.com/86045205/139578779-10c9f5cc-1185-4a45-a500-09b9c02be1ff.png)
  
# TML
Simple library for making multimedia applications.

<h1>Features</h1>
<li>OpenGL 4.5 core</li>
<li>C++14</li>
<li>Everything batch rendered</li>
<li>Simple audio interface</li>
<li>Video playback support using MPEG-1</li>
<li>Custom GUI implementation</li>

<h3>Example</h3>

```c++
#include <TML/TML.h>

using namespace tml;

int main()
{
    Window window(800, 600, "Window title");
    Renderer::Init();
    
    while(!window.ShouldClose())
    {
        Renderer::Clear();
        Renderer::DrawCircle(window.GetSize() / 2, 100.f, RED);
        Renderer::EndBatch();
        window.Display();
    }
    return 0;
}
```

Produces this:

![image](https://user-images.githubusercontent.com/86045205/139579225-5f777849-0926-4470-865a-2983bd15765e.png)
