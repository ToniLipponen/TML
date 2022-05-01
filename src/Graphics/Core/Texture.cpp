#include <TML/Graphics/Core/Texture.h>
#include <TML/Graphics/Core/FrameBuffer.h>
#include "../../Headers/GLHeader.h"
#include "../../Window/GLContext/GLContext.h"

namespace tml
{
    Texture::Texture()
    : m_id(0), m_width(0), m_height(0), m_bpp(0), m_internalFormat(0), m_format(0)
    {

    }

    Texture::Texture(Texture&& rhs) noexcept
    : m_id(rhs.m_id),
      m_width(rhs.m_width),
      m_height(rhs.m_height),
      m_bpp(rhs.m_bpp),
      m_internalFormat(rhs.m_internalFormat),
      m_format(rhs.m_format)
    {
        rhs.m_id = 0;
    }

    Texture::~Texture()
    {
        if(glad_glDeleteTextures)
        {
            GL_CALL(glad_glDeleteTextures(1, &m_id));
        }
    }

    Texture& Texture::operator=(const Texture& rhs) noexcept
    {
        Image image = rhs.GetData();
        LoadFromImage(image);
        return *this;
    }

    Texture& Texture::operator=(Texture&& rhs) noexcept
    {
        m_width     = rhs.m_width;
        m_height    = rhs.m_height;
        m_bpp       = rhs.m_bpp;
        m_id        = rhs.m_id;
        m_minFilter = rhs.m_minFilter;
        m_magFilter = rhs.m_magFilter;
        m_clampMode = rhs.m_clampMode;

        rhs.m_width     = 0;
        rhs.m_height    = 0;
        rhs.m_bpp       = 0;
        rhs.m_id        = 0;
        rhs.m_minFilter = LinearMipmapLinear;
        rhs.m_magFilter = LinearMipmapLinear;
        rhs.m_clampMode = ClampToEdge;

        return *this;
    }
    
    bool Texture::LoadFromImage(const Image& image) noexcept
    {
        return LoadFromMemory(image.GetWidth(), image.GetHeight(), image.GetBpp(), image.GetData());
    }

    bool Texture::LoadFromFile(const String& filename) noexcept
    {
        Image img(filename);
        img.FlipVertically();
        return LoadFromImage(img);
    }

    bool Texture::LoadFromMemory(int32_t w, int32_t h, uint8_t bpp, const uint8_t* data) noexcept
    {
        /**
         *  Delete the old texture handle, and create a new one.
         */
        GL_CALL(glad_glDeleteTextures(1, &m_id));

        #ifdef TML_USE_GLES
            GL_CALL(glad_glGenTextures(1, &m_id));
        #else
            GL_CALL(glad_glCreateTextures(GL_TEXTURE_2D, 1, &m_id));
        #endif

        bool noErrors = true;

        if(m_id == 0)
            noErrors = false;

        m_width 	= w;
        m_height 	= h;
        m_bpp 		= bpp;

        switch(m_bpp)
        {
            case 1:  m_internalFormat = GL_R8;    m_format = GL_RED;                   break;
            case 2:  m_internalFormat = GL_RG8;   m_format = GL_RG;                    break;
            case 3:  m_internalFormat = GL_RGB8;  m_format = GL_RGB;                   break;
            case 4:  m_internalFormat = GL_RGBA8; m_format = GL_RGBA;                  break;
            default: m_internalFormat = GL_RGB8;  m_format = GL_RGB; noErrors = false; break;
        }

        Upload(data);

        return noErrors;
    }

    void Texture::Bind(uint32_t slot) const noexcept
    {
#if defined(TML_USE_GLES) || defined(TML_DONT_USE_DSA)
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

    Image Texture::GetData() const noexcept
    {
        Image image;
        image.LoadFromMemory(m_width, m_height, m_bpp, nullptr);
        auto* imgData = image.GetData();

        #ifndef TML_USE_GLES
            GL_CALL(glad_glGetTextureImage(m_id, 0, m_format, GL_UNSIGNED_BYTE, m_width*m_height*m_bpp, imgData));
        #else
            Bind();
            FrameBuffer frameBuffer;
            frameBuffer.AttachTexture((*this));
            frameBuffer.Bind();
            GL_CALL(glad_glReadPixels(0, 0, m_width, m_height, m_format, GL_UNSIGNED_BYTE, imgData));
        #endif

        return image;
    }

    inline void Texture::Update() const noexcept
    {
        if(m_id == 0)
            return;

#ifdef TML_USE_GLES
        GL_CALL(glad_glBindTexture(GL_TEXTURE_2D, m_id));
        GL_CALL(glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CALL(glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
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
            return;

        Update();

        if(m_width > 0 && m_height > 0)
        {
#ifdef TML_USE_GLES
            GL_CALL(glad_glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, data));
            GL_CALL(glad_glGenerateMipmap(GL_TEXTURE_2D));
#else
            GL_CALL(glad_glTextureStorage2D(m_id, 8, m_internalFormat, m_width, m_height));
            GL_CALL(glad_glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, m_format, GL_UNSIGNED_BYTE, data));
            GL_CALL(glad_glGenerateTextureMipmap(m_id));
#endif
        }
    }
}