#include "../include/Font.h"
#include "../include/Assert.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION 1
#include "../include/stb/stb_image_write.h"
#include <cstdlib>
#include <freetype2/ft2build.h>
#include <ft2build.h>
#include FT_FREETYPE_H

static FT_Library ft;
static bool init = false;

#define ATLAS_SIZE 2048

static void FontInit()
{
    if(!init)
    {
        if(FT_Init_FreeType(&ft))
        {
            tl::Logger::ErrorMessage("Failed to init freetype!");
        }
        else 
        {
            init = true;
        }
    }
}

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

void FlipAtlas(const unsigned char* input,
                           const int width,
                           const int height,
                           const int channel,
                           unsigned char*& output){
  
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width * channel; col += 1) {
            output[row*width+col]=input[row*width+(width-col)];
        }
    }
}


Font::Font()
{
    FontInit();
}

Font::Font(const std::string& filename)
{
    FontInit();
    LoadFromFile(filename);
}

void Font::LoadFromFile(const std::string &filename)
{
    FT_Face face;
    if(FT_New_Face(ft, filename.c_str(), 0, &face))
    {
        tl::Logger::ErrorMessage("Failed to load font -> %s", filename.c_str());
        // exit(1);
    }
    FT_Set_Pixel_Sizes(face, 0, 128);
    ui8* atlas = new ui8[ATLAS_SIZE*ATLAS_SIZE];
    int x = 0, y = ATLAS_SIZE;
    for(i8 c = 32; c < 127; ++c)
    {
        if(FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            tl::Logger::InfoMessage("Failed to load glyph -> %c", c);
            continue;
        }
        FontChar fc = {
            Vector2{face->glyph->bitmap.width, face->glyph->bitmap.rows},
            Vector2{static_cast<float>(x), static_cast<float>(y-face->glyph->bitmap.rows)},
            face->glyph->advance.x,
            face->glyph->bitmap_top
        };
        if(x + face->glyph->bitmap.width < ATLAS_SIZE)
        {
            AddCharToAtlas(atlas, face->glyph->bitmap.buffer, face->glyph->bitmap.width, face->glyph->bitmap.rows, x, y-face->glyph->bitmap.rows);
            x += face->glyph->bitmap.width;
        }
        else 
        {
            y -= face->glyph->bitmap.rows+face->glyph->bitmap_top;
            x = 0;
            AddCharToAtlas(atlas, face->glyph->bitmap.buffer, face->glyph->bitmap.width, face->glyph->bitmap.rows, x, y-face->glyph->bitmap.rows);
        }
        
        m_chars.insert(std::pair<char, FontChar>(c, fc));
    }
    
    m_texture.LoadFromMemory(ATLAS_SIZE, ATLAS_SIZE, 1, atlas);
    stbi_write_png("output.png", ATLAS_SIZE, ATLAS_SIZE,1, atlas, 0);
    FT_Done_Face(face);
}