#include "../include/Drawable/Video.h"

#define PL_MPEG_IMPLEMENTATION
#include "../external-headers/PL_MPEG/pl_mpeg.h"

using namespace tml;

Video::Video()
{

}

Video::Video(cstring filename)
{
    LoadFromFile(filename);
}

Video::~Video()
{
    if(m_stream != nullptr)
        plm_destroy((plm_t*)m_stream);
}

void Video::LoadFromFile(cstring filename)
{
    if(m_stream != nullptr)
        plm_destroy((plm_t*)m_stream);

    m_stream = (plm_t*)plm_create_with_filename(filename);
    if(m_stream)
    {
        ui32 w = plm_get_width((plm_t*)m_stream);
        ui32 h = plm_get_height((plm_t*)m_stream);
        m_size = Vector2(w,h);
    }
}

void Video::Advance(f32 step)
{
    // Todo: Fix whatever is causing GL_INVALID_OPERATION in Texture::Generate()
    ui8* rgb_buffer = new ui8[static_cast<ui32>(m_size.x * m_size.y * 3)]; // Danger
    auto frame = plm_decode_video((plm_t*)m_stream);
    plm_frame_to_rgb(frame, rgb_buffer, m_size.x * 3);
    m_tex.LoadFromMemory(static_cast<int>(m_size.x), static_cast<int>(m_size.y), 3, rgb_buffer);
    delete[] rgb_buffer;
}