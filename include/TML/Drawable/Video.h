#pragma once
#include "Drawable.h"

namespace tml
{
    class Video : public Drawable
    {
    public:
        Video();
        explicit Video(cstring filename) noexcept;
        virtual ~Video();

        void LoadFromFile(cstring filename) noexcept;
        void SetFrameRate(double fps) noexcept;
        void SetLooping(bool loop) const noexcept;
        void Advance(f64 step) noexcept;
        friend class Renderer;
    private:
        void* m_stream = nullptr;
        double m_frame_rate = 30;
        double m_timer = 0;
        double m_one_divided_by_frame_rate = 0;
        i32 m_stream_width = 0; // Dimensions of the actual video
        i32 m_stream_height = 0;
        ui8* m_frame_data;
    };
}