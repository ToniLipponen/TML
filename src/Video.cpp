#include "../include/Drawable/Video.h"
#include "../include/Utilities/Utilities.h"
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
    if(m_frame_data)
        delete[] m_frame_data;
}

void Video::LoadFromFile(cstring filename)
{
    if(m_stream != nullptr)
        plm_destroy((plm_t*)m_stream);

    m_stream = (plm_t*)plm_create_with_filename(filename);
    if(m_stream != nullptr)
    {
        const ui32 w = plm_get_width((plm_t*)m_stream);
        const ui32 h = plm_get_height((plm_t*)m_stream);
        m_size = Vector2(w,h);
        m_frame_rate = plm_get_framerate((plm_t*)m_stream);
//        m_frame_rate = 30.0;
        m_frame_data = new ui8[w*h*3];
    }
}

void Video::Advance(f64 step)
{
    const double d = 1.0 / m_frame_rate;
    m_timer += step;
    if(d < m_timer)
    {
        // Todo: Fix whatever is causing GL_INVALID_OPERATION in Texture::Generate()
        auto frame = plm_decode_video((plm_t*)m_stream);
        plm_frame_to_rgb(frame, m_frame_data, m_size.x * 3);
        m_tex.LoadFromMemory(static_cast<int>(m_size.x), static_cast<int>(m_size.y), 3, m_frame_data);
        m_timer = 0;
    }
}