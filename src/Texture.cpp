#include "../include/Texture.h"
#include "../external-headers/glad/glad.h"
#include "../include/GlDebug.h"
#include "../include/Utilities/Copy.h"
#include "../src/internal/Assert.h"
#include <climits>
#define STB_IMAGE_IMPLEMENTATION
#include "../external-headers/stb/stb_image.h"

#include <algorithm>
using namespace tml;

Texture::Texture()
: m_width(0), m_height(0), m_bpp(0), m_pixeldata(nullptr)
{
    GL_CALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_id));
}

Texture::Texture(cstring filename)
: m_width(0), m_height(0), m_bpp(0), m_pixeldata(nullptr)
{
	GL_CALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_id));
	LoadFromFile(filename);
}

Texture::Texture(i32 w, i32 h, ui8 bpp, ui8* data)
: m_width(w), m_height(h), m_bpp(bpp)
{
    GL_CALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_id));
    LoadFromMemory(w,h,bpp,data);
	Generate();
}

Texture::~Texture()
{
    GL_CALL(glDeleteTextures(1, &m_id));
    if(m_pixeldata != nullptr)
    {
        delete[] m_pixeldata;
        m_pixeldata = nullptr;
    }
}

void Texture::LoadFromFile(cstring filename)
{
	if(m_id == UINT_MAX)
        GL_CALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_id));
	m_pixeldata = stbi_load(filename, &m_width, &m_height, &m_bpp, 0);
	if(m_pixeldata == nullptr)
		tml::Logger::WarningMessage("Failed to load texture -> %s", filename);
	Generate();
}

void Texture::LoadFromMemory(i32 w, i32 h, ui8 bpp, ui8* data)
{
    if(m_allocated) // If memory has been allocated internally, delete the memory
        delete[] m_pixeldata;
    m_pixeldata = data;
    m_width 	= w;
    m_height 	= h;
    m_bpp 		= bpp;
    m_allocated = false;
    Generate();
}

void Texture::CopyFromMemory(i32 w, i32 h, ui8 bpp, ui8* data)
{
    if(m_pixeldata && m_allocated)
        delete[] m_pixeldata;
	m_pixeldata = new ui8[w*h*bpp];
	tl::copy(m_pixeldata, data, w*h*bpp);
	m_width 	= w;
	m_height 	= h;
	m_bpp 		= bpp;
    m_allocated = true;
	Generate();
}

void Texture::Bind(ui32 slot)
{
    GL_CALL(glBindTextureUnit(slot, m_id));
}

void Texture::SetMipMapLevel(ui8 level)
{
	m_mipmap_level = level;
}

void Texture::SetMinMagFilter(Filter min, Filter mag)
{
	m_minfilter = min;
	m_magfilter = mag;
}

void Texture::SetClampMode(ClampMode mode)
{
	m_clampmode = mode;
}

void Texture::Generate(void)
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
