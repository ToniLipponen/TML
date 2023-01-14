#include <TML/Graphics/Core/Texture.h>
#include <TML/Graphics/Core/FrameBuffer.h>
#include "../Headers/GLHeader.h"

namespace tml
{
    Texture::Texture() noexcept
    : m_id(0),
      m_width(0),
      m_height(0),
      m_channels(0),
      m_bitsPerChannel(0),
      m_internalFormat(0),
      m_format(0)
    {

    }

    Texture::Texture(Texture&& rhs) noexcept
    : m_id(rhs.m_id),
      m_width(rhs.m_width),
      m_height(rhs.m_height),
      m_channels(rhs.m_channels),
      m_bitsPerChannel(rhs.m_bitsPerChannel),
      m_internalFormat(rhs.m_internalFormat),
      m_format(rhs.m_format)
    {
        rhs.m_id = 0;
    }

    Texture::~Texture() noexcept
    {
        /// Todo: Figure out why this causes access violation on application exit on Windows.
        GL_CALL(glad_glDeleteTextures(1, &m_id));
    }

    Texture& Texture::operator=(const Texture& rhs) noexcept
    {
        Image image = rhs.GetData();
        m_magFilter = rhs.m_magFilter;
        m_minFilter = rhs.m_minFilter;
        m_clampMode = rhs.m_clampMode;
        LoadFromImage(image);
        return *this;
    }

    Texture& Texture::operator=(Texture&& rhs) noexcept
    {
        std::swap(m_width, rhs.m_width);
        std::swap(m_height, rhs.m_height);
        std::swap(m_channels, rhs.m_channels);
        std::swap(m_bitsPerChannel, rhs.m_bitsPerChannel);
        std::swap(m_id, rhs.m_id);
        std::swap(m_minFilter, rhs.m_minFilter);
        std::swap(m_magFilter, rhs.m_magFilter);
        std::swap(m_clampMode, rhs.m_clampMode);
        rhs.m_id = 0;
        return *this;
    }
    
    bool Texture::LoadFromImage(const Image& image) noexcept
    {
        return LoadFromMemory(image.GetWidth(),
                              image.GetHeight(),
                              4,
                              8,
                              image.GetData());
    }

    bool Texture::LoadFromFile(const String& filename) noexcept
    {
        Image img(filename);
        img.FlipVertically();
        return LoadFromImage(img);
    }

    bool Texture::LoadFromMemory(int32_t w, int32_t h, int32_t channels, int32_t bitsPerChannel, const void* data) noexcept
    {
        GL_CALL(glad_glDeleteTextures(1, &m_id));

#if defined(TML_USE_GLES) || !defined(TML_USE_DSA)
        GL_CALL(glad_glGenTextures(1, &m_id));
#else
        GL_CALL(glad_glCreateTextures(GL_TEXTURE_2D, 1, &m_id));
#endif

        bool noErrors = m_id != 0;

        m_width = w;
        m_height = h;
        m_channels = channels;
        m_bitsPerChannel = bitsPerChannel;

		if(m_width < 1 || m_height < 1 || m_channels < 1 || !(m_bitsPerChannel != 8 || m_bitsPerChannel != 16))
		{
			return false;
		}

        switch(m_bitsPerChannel)
        {
            case 8:
            switch(channels)
            {
                case 1: m_internalFormat = GL_R8;       m_format = GL_RED; 		break;
                case 2: m_internalFormat = GL_RG8;      m_format = GL_RG; 		break;
                case 3: m_internalFormat = GL_RGB8;     m_format = GL_RGB;		break;
                case 4: m_internalFormat = GL_RGBA8;    m_format = GL_RGBA; 	break;
                default:
#ifndef TML_NO_DEBUGGING
                    static_assert(1, "[Error]: Invalid texture channel count");
#endif
					return false;
            } break;

            case 16:
                switch(channels)
                {
                    case 1: m_internalFormat = GL_R16;       m_format = GL_RED; 	break;
                    case 2: m_internalFormat = GL_RG16;      m_format = GL_RG; 		break;
                    case 3: m_internalFormat = GL_RGB16;     m_format = GL_RGB; 	break;
                    case 4: m_internalFormat = GL_RGBA16;    m_format = GL_RGBA; 	break;
                    default:
#ifndef TML_NO_DEBUGGING
                        static_assert(1, "[Error]: Invalid texture channel count");
#endif
						return false;
                } break;

            case 32:
                switch(channels)
                {
                    case 1: m_internalFormat = GL_R32F;       m_format = GL_RED; 	break;
                    case 2: m_internalFormat = GL_RG32F;      m_format = GL_RG; 	break;
                    case 3: m_internalFormat = GL_RGB32F;     m_format = GL_RGB; 	break;
                    case 4: m_internalFormat = GL_RGBA32F;    m_format = GL_RGBA; 	break;
                    default:
#ifndef TML_NO_DEBUGGING
                        static_assert(1, "[Error]: Invalid texture channel count");
#endif
						return false;
                } break;
            default:
#ifndef TML_NO_DEBUGGING
            	static_assert(1, "[Error]: Invalid texture bits per channel");
#endif
				return false;
        }

		if(noErrors)
		{
        	Upload(data);
		}

        return noErrors;
    }

    void Texture::Bind(uint32_t slot) const noexcept
    {
#if defined(TML_USE_GLES) || !defined(TML_USE_DSA)
        GL_CALL(glad_glActiveTexture(GL_TEXTURE0 + slot));
        GL_CALL(glad_glBindTexture(GL_TEXTURE_2D, m_id));
#else
        GL_CALL(glad_glBindTextureUnit(slot, m_id));
#endif
    }

    void Texture::BindToImageSlot(uint32_t slot) const noexcept
    {
        GL_CALL(glad_glBindImageTexture(slot, m_id, 0, GL_FALSE, 0, GL_READ_WRITE, m_internalFormat));
    }

    void Texture::SetMinMagFilter(Filter min, Filter mag) noexcept
    {
        m_minFilter = min;
        m_magFilter = mag;
        Update();
    }

    void Texture::SetClampMode(ClampMode mode) noexcept
    {
        m_clampMode = mode;
        Update();
    }

    [[nodiscard, maybe_unused]]
    Texture::Filter Texture::GetMinFilter() const noexcept
    {
        return m_minFilter;
    }

    [[nodiscard, maybe_unused]]
    Texture::Filter Texture::GetMagFilter() const noexcept
    {
        return m_magFilter;
    }

    [[nodiscard, maybe_unused]]
    Texture::ClampMode Texture::GetClampMode() const noexcept
    {
        return m_clampMode;
    }

    [[nodiscard, maybe_unused]]
    uint32_t Texture::GetID() const noexcept
    {
        return m_id;
    }

    [[nodiscard, maybe_unused]]
    uint32_t Texture::GetWidth() const noexcept
    {
        return m_width;
    }

    [[nodiscard, maybe_unused]]
    uint32_t Texture::GetHeight() const noexcept
    {
        return m_height;
    }

    [[nodiscard, maybe_unused]]
    uint32_t Texture::GetBpp() const noexcept
    {
        return m_bitsPerChannel * m_channels / 8;
    }

    Image Texture::GetData() const noexcept
    {
        Image image;
        image.LoadFromMemory(m_width, m_height, nullptr);
        auto* imgData = image.GetData();

#if defined(TML_USE_GLES) || !defined(TML_USE_DSA)
        Bind();
        FrameBuffer frameBuffer;
        frameBuffer.AttachTexture((*this));
        frameBuffer.Bind();
        GL_CALL(glad_glReadPixels(0, 0, m_width, m_height, m_format, GL_UNSIGNED_BYTE, imgData));
#else
        GL_CALL(glad_glGetTextureImage(m_id, 0, m_format, GL_UNSIGNED_BYTE, m_width * m_height * GetBpp(), imgData));
#endif

        return image;
    }

    inline void Texture::Update() const noexcept
    {
        if(m_id == 0)
        {
            return;
        }

#if defined(TML_USE_GLES) || !defined(TML_USE_DSA)
        GL_CALL(glad_glBindTexture(GL_TEXTURE_2D, m_id));
        GL_CALL(glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_clampMode));
        GL_CALL(glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_clampMode));
        GL_CALL(glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter));
        GL_CALL(glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter));
        GL_CALL(glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 8));
#else
        GL_CALL(glad_glTextureParameteri(m_id, GL_TEXTURE_WRAP_S,     m_clampMode));
        GL_CALL(glad_glTextureParameteri(m_id, GL_TEXTURE_WRAP_T,     m_clampMode));
        GL_CALL(glad_glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, m_minFilter));
        GL_CALL(glad_glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, m_magFilter));
        GL_CALL(glad_glTextureParameteri(m_id, GL_TEXTURE_MAX_LEVEL, 8));
#endif
    }

    inline void Texture::Upload(const void* data) const noexcept
    {
        if(m_id == 0)
        {
            return;
        }

        int type;

        switch(m_bitsPerChannel)
        {
            case 8: 	type = GL_UNSIGNED_BYTE; 	break;
            case 16: 	type = GL_UNSIGNED_SHORT; 	break;
            case 32: 	type = GL_FLOAT; 			break;
            default:								break;
        }

        Update();

        if(m_width > 0 && m_height > 0)
        {
#if defined(TML_USE_GLES) || !defined(TML_USE_DSA)
            GL_CALL(glad_glTexStorage2D(GL_TEXTURE_2D, 8, m_internalFormat, m_width, m_height));
            GL_CALL(glad_glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, m_format, type, data));
            GL_CALL(glad_glGenerateMipmap(GL_TEXTURE_2D));
#else
            GL_CALL(glad_glTextureStorage2D(m_id, 8, m_internalFormat, m_width, m_height));
            GL_CALL(glad_glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, m_format, type, data));
            GL_CALL(glad_glGenerateTextureMipmap(m_id));
#endif
        }
    }
}