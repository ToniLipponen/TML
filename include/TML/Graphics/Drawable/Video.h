#pragma once
#include "Drawable.h"
#include <TML/Graphics/Core/Texture.h>

/*
 * Video playback is brought to you by Dominic Szablewskis PL_MPEG (https://github.com/phoboslab/pl_mpeg).
 * This only supports MPEG1. I would be open to extending support to other video formats
 * through other libraries, if they use license compatible with Zlib license.
 */

namespace tml
{
    class Video : public Drawable
    {
    public:
        Video();
        explicit Video(const std::string& fileName) noexcept;
        virtual ~Video();

        bool LoadFromFile(const std::string& fileName) noexcept;
        void SetFrameRate(double fps) noexcept;
        void SetLooping(bool loop) const noexcept;
        bool Advance(double step) noexcept; ///@brief Returns false when no frames are available.
        friend class Renderer;
    private:
        void OnDraw(class Renderer*, Texture*) noexcept override;
        void* m_stream = nullptr;
        double m_frameRate = 30;
        double m_timer = 1;
        double m_oneDividedByFrameRate = 1.0 / m_frameRate;
        bool m_ended = false;
        i32 m_streamWidth = 0; // Dimensions of the actual video
        i32 m_streamHeight = 0;
        Texture m_y, m_cb, m_cr;
    };
}