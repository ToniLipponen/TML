#pragma once
#include "Drawable.h"

namespace tml
{
    class Video : public Drawable
    {
    public:
        Video();
        Video(cstring filename);
        virtual ~Video();

        void LoadFromFile(cstring filename);
        // Use delta time to playback at normal speed, or multiply delta to control speed.
        void Advance(f64 step);
        friend class Renderer;
    private:
        void* m_stream = nullptr;
        double m_frame_rate = 30;
        double m_timer = 0;
        ui8* m_frame_data = nullptr;
    };
}