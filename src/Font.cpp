#include "../include/Font.h"
#include "../include/Assert.h"

#include <cstdlib>
#include <ft2build.h>
#include FT_FREETYPE_H

#define ATLAS_SIZE 2048

inline constexpr static void AddCharToAtlas(ui8* atlasData, ui8* charData, ui32 w, ui32 h, ui32 x, ui32 y)
{
    for(int i = y; i < y+h; ++i)
    {
        for(int j = x; j < x+w; ++j)
        {
            atlasData[i*ATLAS_SIZE+j] = charData[(i-y)*w + (j-x)];
        }
    }
}

Font::Font()
{
}

Font::Font(const std::string& filename)
{
    LoadFromFile(filename);
}

void Font::LoadFromFile(const std::string &filename)
{
    FT_Library ft;
    FT_Init_FreeType(&ft);
    FT_Face face;
    if(FT_New_Face(ft, filename.c_str(), 0, &face))
    {
        tl::Logger::ErrorMessage("Failed to load font -> %s", filename.c_str());
        exit(1);
    }
    FT_Set_Pixel_Sizes(face, 0, 256);
    ui8* atlas = new ui8[ATLAS_SIZE*ATLAS_SIZE];
    int x = 0, y = ATLAS_SIZE;
    for(i8 c = 32; c < 127; ++c)
    {
        if(FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            tl::Logger::InfoMessage("Failed to load glyph -> %c", c);
            continue;
        }
        FontChar fc;
        if(x + face->glyph->bitmap.width < ATLAS_SIZE)
        {
            fc = {
                Vector2{face->glyph->bitmap.width*1.f, face->glyph->bitmap.rows*1.f},
                Vector2{static_cast<float>(x), static_cast<float>(y-face->glyph->bitmap.rows)},
                face->glyph->advance.x,
                face->glyph->bitmap_top
            };
            AddCharToAtlas(atlas, face->glyph->bitmap.buffer, face->glyph->bitmap.width, face->glyph->bitmap.rows, x, y-face->glyph->bitmap.rows);
            x += face->glyph->bitmap.width;
        }
        else 
        {
            y -= face->glyph->bitmap.rows+face->glyph->bitmap_top;
            x = 0;
            fc = {
                Vector2{face->glyph->bitmap.width*1.f, face->glyph->bitmap.rows*1.f},
                Vector2{static_cast<float>(x), static_cast<float>(y-face->glyph->bitmap.rows)},
                face->glyph->advance.x,
                face->glyph->bitmap_top
            };
            AddCharToAtlas(atlas, face->glyph->bitmap.buffer, face->glyph->bitmap.width, face->glyph->bitmap.rows, x, y-face->glyph->bitmap.rows);
            x += face->glyph->bitmap.width;
        }
        
        m_chars.insert(std::pair<char, FontChar>(c, fc));
    }
    
    m_texture.LoadFromMemory(ATLAS_SIZE, ATLAS_SIZE, 1, atlas);
    delete[] atlas;
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}