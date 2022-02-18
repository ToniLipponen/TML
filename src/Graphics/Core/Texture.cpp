#include <TML/Graphics/Core/Texture.h>
#include <TML/Graphics/Core/RenderTarget.h>
#include "GLHeader.h"
#include "GlDebug.h"

namespace tml
{
    Texture::Texture()
    : m_width(0), m_height(0), m_bpp(0), m_pixelData(nullptr), m_id(0)
    {

    }

    Texture::~Texture()
    {
        GL_CALL(glDeleteTextures(1, &m_id));
    }

    void Texture::LoadFromImage(Image& image)
    {
        LoadFromMemory(image.GetWidth(), image.GetHeight(), image.GetBpp(), image.GetData());
    }

    void Texture::LoadFromFile(const std::string& filename)
    {
        Image img(filename);
        LoadFromImage(img);
    }

    void Texture::LoadFromMemory(i32 w, i32 h, ui8 bpp, ui8* data)
    {
        GL_CALL(glDeleteTextures(1, &m_id));
        #ifdef TML_USE_GLES
            GL_CALL(glGenTextures(1, &m_id));
        #else
            GL_CALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_id));
        #endif
        m_pixelData = data;
        m_width 	= w;
        m_height 	= h;
        m_bpp 		= bpp;
        Generate();
    }

    void Texture::Bind(ui32 slot) const
    {
        #ifdef TML_USE_GLES
            GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
            GL_CALL(glBindTexture(GL_TEXTURE_2D, m_id));
        #else
            GL_CALL(glBindTextureUnit(slot, m_id));
        #endif
    }

    void Texture::SetMinMagFilter(Filter min, Filter mag)
    {
        m_minFilter = min;
        m_magFilter = mag;
        Generate();
    }

    void Texture::SetClampMode(ClampMode mode)
    {
        m_clampMode = mode;
        Generate();
    }

    void Texture::GetData(Image& image) noexcept
    {
        // Doing it like this to avoid one malloc and one copy.
        image.LoadFromMemory(m_width, m_height, 4, nullptr);
        auto* imgData = image.GetData();
        Bind();

        #ifndef TML_USE_GLES
            GL_CALL(glad_glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData));
        #else
            RenderTarget target;
            target.AttachTexture((*this));
            target.Bind();
            GL_CALL(glad_glReadPixels(0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, imgData));
        #endif
    }

    inline void Texture::Generate() const noexcept
    {
        if(m_id == 0)
            return;
        #ifdef TML_USE_GLES
            GL_CALL(glBindTexture(GL_TEXTURE_2D, m_id));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 8));
        #else
            GL_CALL(glTextureParameteri(m_id, GL_TEXTURE_WRAP_S,     m_clampMode));
            GL_CALL(glTextureParameteri(m_id, GL_TEXTURE_WRAP_T,     m_clampMode));
            GL_CALL(glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, m_minFilter));
            GL_CALL(glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, m_magFilter));
            GL_CALL(glTextureParameteri(m_id, GL_TEXTURE_MAX_LEVEL, 8));
        #endif

        if(m_width > 0 && m_height > 0)
        {
            i32 ch = 0, chi = 0; // Format & internal format
            switch(m_bpp)
            {
                case 1: ch = GL_R8;     chi = GL_RED;   break;
                case 2: ch = GL_RG8;    chi = GL_RG;    break;
                case 3: ch = GL_RGB8;   chi = GL_RGB;   break;
                case 4: ch = GL_RGBA8;  chi = GL_RGBA;  break;
                default:                                break;
            }
            #ifdef TML_USE_GLES
                GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, ch, m_width, m_height, 0, chi, GL_UNSIGNED_BYTE, m_pixelData));
                GL_CALL(glBindImageTexture(0, m_id, 0, GL_FALSE, 0, GL_READ_WRITE, ch));
                GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
            #else
                GL_CALL(glTextureStorage2D(m_id, 8, ch, m_width, m_height));
                GL_CALL(glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, chi, GL_UNSIGNED_BYTE, m_pixelData));
                GL_CALL(glBindImageTexture(0, m_id, 0, GL_FALSE, 0, GL_READ_WRITE, ch));
                GL_CALL(glGenerateTextureMipmap(m_id));
            #endif
        }
    }
}