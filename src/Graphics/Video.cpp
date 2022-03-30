#include <TML/Graphics/Drawable/Video.h>
#include <TML/Graphics/Renderer.h>
#include "../Headers/_Assert.h"
#define PL_MPEG_IMPLEMENTATION 1
#include <PL_MPEG/pl_mpeg.h>

/*
 * No audio at the moment.
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
    }

    bool Video::LoadFromFile(const std::string& filename) noexcept
    {
        if(m_stream != nullptr)
            plm_destroy(static_cast<plm_t*>(m_stream));

        m_stream = plm_create_with_filename(filename.c_str());

        if(m_stream != nullptr)
        {
            m_pos = Vector2f{0};
            m_streamWidth  = plm_get_width(static_cast<plm_t*>(m_stream));
            m_streamHeight = plm_get_height(static_cast<plm_t*>(m_stream));
            m_size = Vector2f(static_cast<float>(m_streamWidth), static_cast<float>(m_streamHeight));

            SetFrameRate(plm_get_framerate(static_cast<plm_t*>(m_stream)));
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

    bool Video::Advance(f64 step) noexcept
    {
        m_ended = static_cast<bool>(plm_has_ended(reinterpret_cast<plm_t*>(m_stream)));
        if((m_oneDividedByFrameRate < (m_timer += step)) && !m_ended)
        {
            const plm_frame_t* frame = plm_decode_video(reinterpret_cast<plm_t*>(m_stream));
            if(frame)
            {
                m_y.LoadFromMemory( frame->y.width,  frame->y.height,  1, frame->y.data);
                m_cb.LoadFromMemory(frame->cb.width, frame->cb.height, 1, frame->cb.data);
                m_cr.LoadFromMemory(frame->cr.width, frame->cr.height, 1, frame->cr.data);
            }
            m_timer = 0;
        }
        return !m_ended;
    }

    void Video::OnDraw(Renderer* renderer, Texture *) noexcept
    {
        renderer->PushQuad(m_pos, m_size, m_color, m_y, Vertex::VIDEO);
        renderer->PushTexture(m_cb);
        renderer->PushTexture(m_cr);
    }
}
