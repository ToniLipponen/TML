#include <TML/System/Clipboard.h>
#define GLFW_INCLUDE_NONE
#include <clip.h>
#include <iostream>

namespace tml
{
    bool Clipboard::IsEmpty()
    {
        return clip::has(clip::image_format()) || clip::has(clip::image_format());
    }

    bool Clipboard::HasImage()
    {
        return clip::has(clip::image_format());
    }

    bool Clipboard::HasText()
    {
        return clip::has(clip::image_format());
    }

    bool Clipboard::GetString(String& string)
    {
        if(HasText())
        {
            std::string str;
            clip::get_text(str);
            string = str;
            return true;
        }
        return false;
    }

    bool Clipboard::GetImage(Image& image)
    {
        if(HasImage())
        {
            clip::image clipImage;
            clip::get_image(clipImage);

            image.LoadFromMemory(
                static_cast<i32>(clipImage.spec().width),
                static_cast<i32>(clipImage.spec().height),
                static_cast<i32>(clipImage.spec().bits_per_pixel / 8),
                (ui8*)clipImage.data()
            );
            return true;
        }
        return false;
    }

    void Clipboard::Clear()
    {
        clip::clear();
    }

    void Clipboard::SetString(const String& string)
    {
        clip::set_text(string.cpp_str());
    }

    void Clipboard::SetImage(const Image& image)
    {
        clip::image_spec spec{1};
        spec.bits_per_pixel = image.GetBpp() * 8;
        spec.width = image.GetWidth();
        spec.height = image.GetHeight();
        spec.bytes_per_row = spec.width * image.GetBpp();

        spec.red_mask    = 0x000000ff;
        spec.green_mask  = 0x0000ff00;
        spec.blue_mask   = 0x00ff0000;
        spec.red_shift   = 0;
        spec.green_shift = 8;
        spec.blue_shift  = 16;

        clip::image clipImage(image.GetData(), spec);
        clip::set_image(clipImage);
    }
}