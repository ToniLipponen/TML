#pragma once
#include "Drawable.h"

namespace tml
{
    class Video : public Drawable
    {
    public:
        Video();
        Video(const std::string& fileName) noexcept;
        virtual ~Video();

        bool LoadFromFile(const std::string& fileName) noexcept;
        void SetFrameRate(double fps) noexcept;
        void SetLooping(bool loop) const noexcept;
        void Advance(double step) noexcept;
        friend class Renderer;
    private:
        void* m_stream = nullptr;
        double m_frameRate = 30;
        double m_timer = 0;
        double m_oneDividedByFrameRate = 1.0 / m_frameRate;
        i32 m_streamWidth = 0; // Dimensions of the actual video
        i32 m_streamHeight = 0;
        Texture m_y, m_cb, m_cr;
    };
}