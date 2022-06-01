#pragma once
#include <TML/Audio/AudioStream.h>

namespace tml
{
    class TML_API AudioCapture
    {
    public:
        AudioCapture() noexcept;
        ~AudioCapture() noexcept;
    public:
        static AudioCapture& GetInstance() noexcept;
        AudioStream& GetStream() noexcept;
        void Start() noexcept;
        void Stop() noexcept;

    private:
        void* m_device;
        AudioStream m_stream;
    };
}