#pragma once
#include <TML/Audio/AudioBuffer.h>
#include <TML/Audio/AudioType.h>
#include <deque>
#include <mutex>

namespace tml
{
    class TML_API AudioStream : public AudioType
    {
    public:
        AudioStream() noexcept;
        AudioStream& operator<<(AudioStream& rhs) noexcept;
        AudioStream& operator<<(const AudioBuffer& rhs) noexcept;
        AudioStream& operator>>(AudioBuffer& rhs) noexcept;
        AudioStream& operator>>(AudioStream& rhs) noexcept;
        uint64_t GetLength() noexcept override;
        virtual void Flush() noexcept;

    private:
        double GetProgress() const noexcept override { return 0; }
        uint32_t ReadFrames(float* output, uint32_t frameCount) override;

    private:
        std::deque<float> m_buffer;
        std::mutex m_mutex;
    };
}