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
        void Advance(f32 step);
        friend class Renderer;
    private:
        void* m_stream = nullptr;
    };
}