#include "../include/Texture.h"
#include "../include/glad/glad.h"
#include "../include/Assert.h"
#include "../include/Utilities/Copy.h"
#include <climits>
#include <cstring>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb/stb_image.h"

#include <algorithm>

Texture::Texture()
: m_width(0), m_height(0), m_bpp(0), m_pixeldata(nullptr)
{

}

Texture::Texture(cstring filename)
: m_width(0), m_height(0), m_bpp(0), m_pixeldata(nullptr)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
	LoadFromFile(filename);
}

Texture::Texture(i32 w, i32 h, ui8 bpp, ui8* data)
: m_width(w), m_height(h), m_bpp(bpp), m_pixeldata(data)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
	Generate();
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_id);
	delete[] m_pixeldata;
}

void Texture::LoadFromFile(cstring filename)
{
	if(m_id == UINT_MAX)
		glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
	stbi_set_flip_vertically_on_load(1);
	m_pixeldata = stbi_load(filename, &m_width, &m_height, &m_bpp, 0);
	if(m_pixeldata == nullptr)
		tl::Logger::WarningMessage("Failed to load texture -> %s", filename);
	Generate();
}

void Texture::LoadFromMemory(i32 w, i32 h, ui8 bpp, ui8* data)
{
	if(m_id == UINT_MAX)
		glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
	m_pixeldata = new ui8[w*h*bpp];
	tl::copy(m_pixeldata, data, w*h*bpp);
	m_width 	= w;
	m_height 	= h;
	m_bpp 		= bpp;
	Generate();
}

void Texture::Bind(ui32 slot)
{
	glBindTextureUnit(slot, m_id);
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
	glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, m_clampmode);
	glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, m_clampmode);
	glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, m_minfilter);
	glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, m_magfilter);
	glTextureParameterf(m_id, GL_TEXTURE_MAX_ANISOTROPY, 16);
	glTextureParameterf(m_id, GL_TEXTURE_LOD_BIAS, 0);
	
	if(m_width > 0 && m_height > 0)
	{
		if(m_pixeldata != nullptr)
		{
			switch(m_bpp)
			{
			case 1:
				glTextureStorage2D(m_id, 8, GL_R8, m_width, m_height);
				glTextureSubImage2D(m_id, m_mipmap_level, 0, 0, m_width, m_height, GL_RED, GL_UNSIGNED_BYTE, m_pixeldata);
				glGenerateTextureMipmap(m_id);
			break;
			case 2:
				glTextureStorage2D(m_id, 8, GL_RG8, m_width, m_height);
				glTextureSubImage2D(m_id, m_mipmap_level, 0, 0, m_width, m_height, GL_RG, GL_UNSIGNED_BYTE, m_pixeldata);
				glGenerateTextureMipmap(m_id);
			break;
			case 3:
				glTextureStorage2D(m_id, 8, GL_RGB8, m_width, m_height);
				glTextureSubImage2D(m_id, m_mipmap_level, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, m_pixeldata);
				glGenerateTextureMipmap(m_id);
			break;
			case 4:
				glTextureStorage2D(m_id, 8, GL_RGBA8, m_width, m_height);
				glTextureSubImage2D(m_id, m_mipmap_level, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, m_pixeldata);
				glGenerateTextureMipmap(m_id);
			break;
			default:
			break;
			}
		}
	}
}
