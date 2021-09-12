#include <TML/Texture.h>
#include <glad/glad.h>
#include <stb/stb_image.h>
#include <climits>
#include <algorithm>
#include "internal/GlDebug.h"
#include "internal/Assert.h"
using namespace tml;

Texture::Texture()
: m_width(0), m_height(0), m_bpp(0), m_pixeldata(nullptr), m_id(UINT_MAX)
{

}

Texture::~Texture()
{
    GL_CALL(glDeleteTextures(1, &m_id));
}

void Texture::LoadFromImage(Image& image)
{
    if(m_id == UINT_MAX)
        GL_CALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_id));
    m_width = image.GetWidth();
    m_height = image.GetHeight();
    m_bpp = image.GetBpp();
    m_pixeldata = image.GetData();
    Generate();
}

void Texture::LoadFromFile(const std::string& filename)
{
    Image img(filename);
    LoadFromImage(img);
	Generate();
}

void Texture::LoadFromMemory(i32 w, i32 h, ui8 bpp, ui8* data)
{
    if(m_id == UINT_MAX)
        GL_CALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_id));
    m_pixeldata = data;
    m_width 	= w;
    m_height 	= h;
    m_bpp 		= bpp;
    Generate();
}

void Texture::Bind(ui32 slot) const
{
    GL_CALL(glBindTextureUnit(slot, m_id));
}

void Texture::SetMipMapLevel(ui8 level)
{
	m_mipmap_level = level;
    Generate();
}

void Texture::SetMinMagFilter(Filter min, Filter mag)
{
	m_minfilter = min;
	m_magfilter = mag;
    Generate();
}

void Texture::SetClampMode(ClampMode mode)
{
	m_clampmode = mode;
    Generate();
}

void Texture::Generate()
{
    GL_CALL(glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, m_clampmode));
    GL_CALL(glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, m_clampmode));
    GL_CALL(glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, m_minfilter));
    GL_CALL(glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, m_magfilter));
	
	if(m_width > 0 && m_height > 0)
	{
		if(m_pixeldata != nullptr)
		{
            i32 ch = 0, chi = 0;
			switch(m_bpp)
			{
			case 1:
                ch = GL_R8; chi = GL_RED;
			break;
			case 2:
                ch = GL_RG8; chi = GL_RG;
			break;
			case 3:
                ch = GL_RGB8; chi = GL_RGB;
			break;
			case 4:
                ch = GL_RGBA8; chi = GL_RGBA;
			break;
			default:
			break;
			}
            GL_CALL(glTextureStorage2D(m_id, 8, ch, m_width, m_height));
            GL_CALL(glTextureSubImage2D(m_id, m_mipmap_level, 0, 0, m_width, m_height, chi, GL_UNSIGNED_BYTE, m_pixeldata));
            GL_CALL(glGenerateTextureMipmap(m_id));
		}
	}
}
