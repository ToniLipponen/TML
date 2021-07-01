#pragma once
#include "Types.h"
#include "Assert.h"
#include "glm/vec2.hpp"
#include <climits>
#include <unordered_map>

class Texture
{
public:
	enum ClampMode
	{
		ClampToBorder 		= 0x812D,
		ClampToEdge 		= 0x812F,
		MirrorClampToEdge 	= 0x8743,
		Repeat				= 0x2901,
		MirrorRepeat		= 0x8370,
	};
	enum Filter
	{
		Nearest 				= 0x2600,
		Linear 					= 0x2601,
		NearestMipmapNearest 	= 0x2700,
		LinearMipmapNearest 	= 0x2701,
		NearestMipmapLinear		= 0x2702,
		LinearMipmapLinear		= 0x2703,
	};
public:
	Texture();
	Texture(cstring filename);
	Texture(i32 w, i32 h, ui8 bpp, ui8* data);
	~Texture();

	void LoadFromFile(cstring filename);
	void LoadFromMemory(i32 w, i32 h, ui8 bpp, ui8* data);
	void Bind(ui32 slot = 0);
	
	void SetMipMapLevel(ui8 level);
	void SetMinMagFilter(Filter min, Filter mag);
	void SetClampMode(ClampMode mode);
	void SetScale(const glm::vec2& scale) noexcept
	{
		m_scale = scale;
	}
	constexpr glm::vec2 GetScale() const noexcept
	{
		return m_scale;
	}
	constexpr bool HasData() const noexcept
	{
		return m_id != UINT_MAX;
	}
	constexpr ui32 GetID()
	{
		return m_id;
	}
private:
	void Generate(void);

private:
	ClampMode m_clampmode = Repeat;
	Filter m_minfilter = LinearMipmapLinear;
	Filter m_magfilter = Linear;
	ui32 m_id = UINT_MAX;
	i32 m_width, m_height, m_bpp;
	glm::vec2 m_scale = {1.f, 1.f};
	ui8* m_pixeldata;
	ui8 m_mipmap_level = 0;
};
