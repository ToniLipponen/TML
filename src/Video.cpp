#include <TML/Drawable/Video.h>
#include "internal/Assert.h"
#define PL_MPEG_IMPLEMENTATION 1
#include <PL_MPEG/pl_mpeg.h>

/*
 * Currently, conversion from YCrCb data to RGB is done on the CPU.
 * Also, no audio decoding at the moment.
 */

namespace tml
{
    Video::Video() = default;

    Video::Video(const std::string& filename) noexcept
    {
        LoadFromFile(filename);
    }

    Video::~Video()
    {
        if(m_stream != nullptr)
            plm_destroy((plm_t*)m_stream);
        delete[] m_frameData;
    }

    bool Video::LoadFromFile(const std::string& filename) noexcept
    {
        if(m_stream != nullptr)
            plm_destroy((plm_t*)m_stream);

        m_stream = (plm_t*)plm_create_with_filename(filename.c_str());

        if(m_stream != nullptr)
        {
            m_pos = 0;
            m_streamWidth  = plm_get_width((plm_t*)m_stream);
            m_streamHeight = plm_get_height((plm_t*)m_stream);
            m_size = Vector2(static_cast<float>(m_streamWidth), static_cast<float>(m_streamHeight));

            SetFrameRate(plm_get_framerate((plm_t*)m_stream));

            delete[] m_frameData;
            m_frameData    = new ui8[m_streamWidth * m_streamHeight * 3];
            plm_set_audio_enabled(reinterpret_cast<plm_t*>(m_stream), false);
            return true;
        }

        tml::Logger::ErrorMessage("Failed to load video -> %s", filename.c_str());
        return false;
    }

    void Video::SetFrameRate(double r) noexcept
    {
        m_frameRate = fabs(r);
        m_oneDividedByFrameRate = 1.0 / m_frameRate;
    }

    void Video::SetLooping(bool loop) const noexcept
    {
        plm_set_loop(reinterpret_cast<plm_t*>(m_stream), static_cast<int>(loop));
    }

    void Video::Advance(f64 step) noexcept
    {
        if(m_oneDividedByFrameRate < (m_timer += step)
        && plm_has_ended(reinterpret_cast<plm_t*>(m_stream)) != 1)
        {
            plm_frame_t* frame = plm_decode_video(reinterpret_cast<plm_t*>(m_stream));
            plm_frame_to_rgb(frame, m_frameData, m_streamWidth * 3);
            m_tex.LoadFromMemory(m_streamWidth, m_streamHeight, 3, m_frameData);
            m_timer = 0;
        }
    }
}
