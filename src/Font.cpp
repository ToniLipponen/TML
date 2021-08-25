#include "../include/Font.h"
#include "../include/Assert.h"
#define STB_TRUETYPE_IMPLEMENTATION 1
#include "../include/stb/stb_truetype.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION 1
#include "../include/stb/stb_image_write.h"
#include <fstream>

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

Font::Font(){}

Font::Font(const std::string& filename)
{
    LoadFromFile(filename);
}

void Font::LoadFromFile(const std::string& filename)
{
    stbtt_bakedchar cdata[96];

    unsigned char* bitmap = new unsigned char[ATLAS_SIZE*ATLAS_SIZE];
    unsigned char* buffer;
    stbtt_fontinfo font;
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if(!file.is_open())
    {
        tl::Logger::ErrorMessage("Could not load font -> %s", filename.c_str());
        delete[] bitmap;
        file.close();
        return;
    }
    const auto file_len = file.tellg();
    file.seekg(0, std::ios::beg);
    buffer = new unsigned char[file_len];
    file.read(reinterpret_cast<char*>(buffer), file_len);
    file.close();

    stbtt_InitFont(&font, buffer, 0);
    stbtt_BakeFontBitmap(reinterpret_cast<const unsigned char *>(buffer), 0, 256.0, bitmap, ATLAS_SIZE, ATLAS_SIZE, 32, 96, cdata);
    m_texture.LoadFromMemory(ATLAS_SIZE, ATLAS_SIZE, 1, bitmap);
    stbi_write_png("atlas.png", ATLAS_SIZE, ATLAS_SIZE, 1, bitmap, 0);
    for(int i = 0; i < 96; i++)
    {
        m_chars[static_cast<char>(i+32)] = (FontChar){
            {static_cast<float>(cdata[i].x1) / ATLAS_SIZE, static_cast<float>(cdata[i].y1) / ATLAS_SIZE},
            {static_cast<float>(cdata[i].x0) / ATLAS_SIZE, static_cast<float>(cdata[i].y0) / ATLAS_SIZE},
            {static_cast<float>(cdata[i].xoff) / ATLAS_SIZE, static_cast<float>(cdata[i].yoff) / ATLAS_SIZE},
            cdata[i].xadvance};
    }
    delete[] buffer;
    delete[] bitmap;
}
//void Font::LoadFromFile(const std::string &filename)
//{
//    FT_Library ft;
//    FT_Init_FreeType(&ft);
//    FT_Face face;
//    if(FT_New_Face(ft, filename.c_str(), 0, &face))
//    {
//        tl::Logger::ErrorMessage("Failed to load font -> %s", filename.c_str());
//        exit(1);
//    }
//    FT_Set_Pixel_Sizes(face, 0, 256);
//    ui8* atlas = new ui8[ATLAS_SIZE*ATLAS_SIZE];
//    int x = 0, y = ATLAS_SIZE;
//    for(i8 c = 32; c < 127; ++c)
//    {
//        if(FT_Load_Char(face, c, FT_LOAD_RENDER))
//        {
//            tl::Logger::InfoMessage("Failed to load glyph -> %c", c);
//            continue;
//        }
//        FontChar fc;
//        if(x + face->glyph->bitmap.width < ATLAS_SIZE)
//        {
//            fc = {
//                Vector2{face->glyph->bitmap.width*1.f, face->glyph->bitmap.rows*1.f},
//                Vector2{static_cast<float>(x), static_cast<float>(y-face->glyph->bitmap.rows)},
//                static_cast<i32>(face->glyph->advance.x),
//                face->glyph->bitmap_top
//            };
//            AddCharToAtlas(atlas, face->glyph->bitmap.buffer, face->glyph->bitmap.width, face->glyph->bitmap.rows, x, y-face->glyph->bitmap.rows);
//            x += face->glyph->bitmap.width;
//        }
//        else
//        {
//            y -= face->glyph->bitmap.rows+face->glyph->bitmap_top;
//            x = 0;
//            fc = {
//                Vector2{face->glyph->bitmap.width*1.f, face->glyph->bitmap.rows*1.f},
//                Vector2{static_cast<float>(x), static_cast<float>(y-face->glyph->bitmap.rows)},
//                static_cast<i32>(face->glyph->advance.x),
//                face->glyph->bitmap_top
//            };
//            AddCharToAtlas(atlas, face->glyph->bitmap.buffer, face->glyph->bitmap.width, face->glyph->bitmap.rows, x, y-face->glyph->bitmap.rows);
//            x += face->glyph->bitmap.width;
//        }
//
//        m_chars.insert(std::pair<char, FontChar>(c, fc));
//    }
//
//    m_texture.LoadFromMemory(ATLAS_SIZE, ATLAS_SIZE, 1, atlas);
//    delete[] atlas;
//    FT_Done_Face(face);
//    FT_Done_FreeType(ft);
//}