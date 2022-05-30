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
        AudioStream& operator<<(const AudioBuffer& rhs) noexcept;
        AudioBuffer& operator>>(AudioBuffer& rhs) noexcept;
        uint64_t GetLength() noexcept override;

    private:
        double GetProgress() const noexcept override { return 0; }
        uint32_t ReadFrames(float* output, uint32_t frameCount) override;

    private:
        std::deque<float> m_buffer;
        std::mutex m_mutex;
    };
}