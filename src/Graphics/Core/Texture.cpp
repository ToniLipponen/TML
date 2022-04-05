#include <TML/Graphics/Core/Texture.h>
#include <TML/Graphics/Core/FrameBuffer.h>
#include "../../Headers/GLHeader.h"

namespace tml
{
    Texture::Texture()
    : m_id(0), m_width(0), m_height(0), m_bpp(0)
    {

    }

    Texture::Texture(Texture&& rhs) noexcept
    : m_id(rhs.m_id), m_width(rhs.m_width), m_height(rhs.m_height), m_bpp(rhs.m_bpp)
    {
        rhs.m_id = 0;
    }

    Texture::~Texture()
    {
        GL_CALL(glad_glDeleteTextures(1, &m_id));
    }

    Texture& Texture::operator=(const Texture& rhs) noexcept
    {
        Image image;
        rhs.GetData(image);
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
    
    void Texture::LoadFromImage(const Image& image)
    {
        LoadFromMemory(image.GetWidth(), image.GetHeight(), image.GetBpp(), image.GetData());
    }

    void Texture::LoadFromFile(const String& filename)
    {
        Image img(filename);
        img.FlipVertically();
        LoadFromImage(img);
    }

    void Texture::LoadFromMemory(i32 w, i32 h, ui8 bpp, const ui8* data)
    {
        GL_CALL(glad_glDeleteTextures(1, &m_id));

        #ifdef TML_USE_GLES
            GL_CALL(glad_glGenTextures(1, &m_id));
        #else
            GL_CALL(glad_glCreateTextures(GL_TEXTURE_2D, 1, &m_id));
        #endif

        m_width 	= w;
        m_height 	= h;
        m_bpp 		= bpp;
        Upload(data);
    }

    void Texture::Bind(ui32 slot) const
    {
#if defined(TML_USE_GLES) || defined(TML_DONT_USE_DSA)
        GL_CALL(glad_glActiveTexture(GL_TEXTURE0 + slot));
        GL_CALL(glad_glBindTexture(GL_TEXTURE_2D, m_id));
#else
        GL_CALL(glad_glBindTextureUnit(slot, m_id));
#endif
    }

    void Texture::SetMinMagFilter(Filter min, Filter mag)
    {
        m_minFilter = min;
        m_magFilter = mag;
        Update();
    }

    void Texture::SetClampMode(ClampMode mode)
    {
        m_clampMode = mode;
        Update();
    }

    void Texture::GetData(Image& image) const noexcept
    {
        image.LoadFromMemory(m_width, m_height, 4, nullptr);
        auto* imgData = image.GetData();
        Bind();

        #ifndef TML_USE_GLES
            GL_CALL(glad_glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData));
        #else
            FrameBuffer frameBuffer;
            frameBuffer.AttachTexture((*this));
            frameBuffer.Bind();
            GL_CALL(glad_glReadPixels(0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, imgData));
        #endif
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
            i32 ch = 0, chi = 0; // Format & internal format
            switch(m_bpp)
            {
                case 1: ch = GL_R8;     chi = GL_RED;   break;
                case 2: ch = GL_RG8;    chi = GL_RG;    break;
                case 3: ch = GL_RGB8;   chi = GL_RGB;   break;
                case 4: ch = GL_RGBA8;  chi = GL_RGBA;  break;
                default:ch = GL_RGB8;   chi = GL_RGB;   break;
            }
#ifdef TML_USE_GLES
            GL_CALL(glad_glTexImage2D(GL_TEXTURE_2D, 0, ch, m_width, m_height, 0, chi, GL_UNSIGNED_BYTE, data));
            GL_CALL(glad_glGenerateMipmap(GL_TEXTURE_2D));
#else
            GL_CALL(glad_glTextureStorage2D(m_id, 8, ch, m_width, m_height));
            GL_CALL(glad_glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, chi, GL_UNSIGNED_BYTE, data));
            GL_CALL(glad_glBindImageTexture(0, m_id, 0, GL_FALSE, 0, GL_READ_WRITE, ch));
            GL_CALL(glad_glGenerateTextureMipmap(m_id));
#endif
        }
    }
}