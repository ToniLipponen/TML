#pragma once
#include <string>
#include <map>
#include "Types.h"
#include "Vector2.h"
#include "Texture.h"

class Font
{
public:
    Font();
    Font(const std::string& filename);
    void LoadFromFile(const std::string& filename);
    Texture m_texture;
    friend class Text;
private:
    struct FontChar
    {
        Vector2 size;
        Vector2 pos;
        i32 advance;
        i32 top;
    };
    std::map<char, FontChar> m_chars;
};