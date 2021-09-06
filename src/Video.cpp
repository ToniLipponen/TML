#include "../include/Drawable/Video.h"
#include "../include/Utilities/Utilities.h"
#define PL_MPEG_IMPLEMENTATION 1
#include "../external-headers/PL_MPEG/pl_mpeg.h"

using namespace tml;

Video::Video() noexcept
{

}

Video::Video(cstring filename) noexcept
{
    LoadFromFile(filename);
}

Video::~Video()
{
    if(m_stream != nullptr)
        plm_destroy((plm_t*)m_stream);
}

void Video::LoadFromFile(cstring filename) noexcept
{
    if(m_stream != nullptr)
        plm_destroy((plm_t*)m_stream);

    m_stream = (plm_t*)plm_create_with_filename(filename);

    if(m_stream != nullptr)
    {
        m_stream_width  = plm_get_width((plm_t*)m_stream);
        m_stream_height = plm_get_height((plm_t*)m_stream);
        m_size          = Vector2(m_stream_width, m_stream_height);
        SetFrameRate(plm_get_framerate((plm_t*)m_stream));
        m_frame_data    = new ui8[m_stream_width*m_stream_height*3];
        plm_set_audio_enabled(reinterpret_cast<plm_t*>(m_stream), 0);
    }
    else
        tml::Logger::ErrorMessage("Failed to load video -> %s", filename);
}

void Video::SetFrameRate(double r) noexcept
{
    m_frame_rate = fabs(r);
    m_one_divided_by_frame_rate = 1.0 / m_frame_rate;
}

void Video::SetLooping(bool loop) const noexcept
{
    plm_set_loop(reinterpret_cast<plm_t*>(m_stream), static_cast<int>(loop));
}

void Video::Advance(f64 step) noexcept
{
    if(m_one_divided_by_frame_rate < (m_timer += step)
    && plm_has_ended(reinterpret_cast<plm_t*>(m_stream)) != 1)
    {
        // Todo: Fix whatever is causing GL_INVALID_OPERATION in Texture::Generate()
        plm_frame_t * const frame = plm_decode_video(reinterpret_cast<plm_t*>(m_stream));
        plm_frame_to_rgb(frame, m_frame_data, m_stream_width * 3);
        m_tex.LoadFromMemory(m_stream_width, m_stream_height, 3, m_frame_data);
        m_timer = 0;
    }
}