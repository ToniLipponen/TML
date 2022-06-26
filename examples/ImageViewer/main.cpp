/**
  Copyright (C) 2022 Toni Lipponen

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
 */

/**
 * @Usage Very simple image viewer application. You can import images by pasting them in the window with CTRL + V.
 * Or just drag and dropping them in the window.
 * You can zoom in and out with mouse wheel. Press R to reset view. Press F to toggle filtering.
 * This demo might be a bit messy. If you want, you can copy this and make something a bit more polished out of it.
 */

#include <TML/Graphics.h>
#include <TML/Window.h>
#include <TML/System.h>

using namespace tml;

int main(int argc, char** argv)
{
    RenderWindow window(1920, 1080, "TML - Image viewer", WindowSettings::Resizeable | WindowSettings::Maximized);
    Sprite image;
    Vector2f imageSize = 0;
    Camera cam;

    auto scaleImage = [&]()
    {
        const float aspect = imageSize.y / imageSize.x;
        const float window_aspect = (float)window.GetHeight() / (float)window.GetWidth();

        imageSize = image.GetSize();

        if(aspect < window_aspect)
        {
            imageSize.x = window.GetWidth();
            imageSize.y = imageSize.x * aspect;
            image.SetPosition({0, (window.GetHeight() - imageSize.y) / 2});
        }
        else
        {
            imageSize.y = window.GetHeight();
            imageSize.x = imageSize.y / aspect;
            image.SetPosition({(window.GetWidth() - imageSize.x) / 2, 0});
        }

        image.SetSize(imageSize);
    };

    if(argc > 1)
    {
        if(image.LoadFromFile(std::string(argv[1])))
        {
            window.SetTitle(std::string(argv[1]) + " - " + std::to_string(int(image.GetSize().x)) + "x" + std::to_string(int(image.GetSize().y)));
            imageSize = image.GetSize();
        }
    }

    window.SetClearColor(Color(0x444444ff));
    scaleImage();
    Vector2f beginPos = 0;
    Vector2f oldCamPos;
    bool click = false;
    bool filter = true;

    while(window.IsOpen())
    {
        Event windowEvent{};

        while(window.WaitEvents(windowEvent))
        {
            switch(windowEvent.type)
            {
                case EventType::MouseButtonPressed:
                    if(windowEvent.mouseButton.button == Mouse::Button::Left && !click)
                    {
                        click = true;
                        beginPos = Mouse::GetPosition();
                        oldCamPos = cam.GetPosition();
                    }
                    break;

                case EventType::MouseButtonReleased:
                    if(windowEvent.mouseButton.button == Mouse::Button::Left)
                        click = false;

                    break;

                case EventType::MouseMoved:
                    if(click)
                        cam.SetPosition((oldCamPos - (Vector2f(windowEvent.pos.x, windowEvent.pos.y) - beginPos) / cam.GetZoom()));
                    break;

                case EventType::KeyPressed:
                {
                    if(windowEvent.key.value == Keyboard::Key::V && windowEvent.key.control)
                    {
                        if(!Clipboard::IsEmpty())
                        {
                            if(Clipboard::HasText())
                            {
                                String str;
                                Clipboard::GetString(str);
                                Image img;
                                if(img.LoadFromFile(str))
                                {
                                    img.FlipVertically();
                                    image.LoadFromImage(img);
                                    imageSize = image.GetSize();
                                    window.SetTitle(str.cpp_str() + " - " + std::to_string(img.GetWidth()) + " x " + std::to_string(img.GetHeight()));
                                }
                            }
                            else if(Clipboard::HasImage())
                            {
                                Image img;
                                Clipboard::GetImage(img);
                                img.FlipVertically();
                                image.LoadFromImage(img);

                                imageSize = image.GetSize();
                                window.SetTitle(std::to_string(img.GetWidth()) + " x " + std::to_string(img.GetHeight()));
                            }
                        }
                    }
                    else if(windowEvent.key.value == Keyboard::Key::R)
                    {
                        cam.SetPosition({0, 0});
                        cam.SetZoom(1);
                    }
                    else if(windowEvent.key.value == Keyboard::Key::F)
                        image.SetInterpolation(filter = !filter);
                } break;

                case EventType::MouseWheelScrolled:
                {
                    cam.Zoom(windowEvent.mouseWheel.deltaY * cam.GetZoom() / 5);
                    if(cam.GetZoom() < 0.1)
                        cam.SetZoom(0.1);
                } break;

                case EventType::WindowResized:
                    cam.SetPosition({0, 0});
                    break;

                case EventType::Closed:
                    window.Close();
                    break;

                case EventType::Drop:
                {
                    std::vector<String> paths = window.GetDroppedFiles();
                    if(!paths.empty())
                    {
                        Image img;
                        String str = paths.at(0);

                        if(img.LoadFromFile(str))
                        {
                            img.FlipVertically();
                            image.LoadFromImage(img);
                            imageSize = image.GetSize();
                            window.SetTitle(str.cpp_str() + " - " + std::to_string(img.GetWidth()) + " x " + std::to_string(img.GetHeight()));
                        }
                    }
                } break;

                default:
                    break;
            }

            scaleImage();
            window.Clear();
            window.SetCamera(cam);
            window.Draw(image);
            window.Display();
        }
    }
    return 0;
}