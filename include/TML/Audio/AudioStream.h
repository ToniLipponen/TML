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
        size_t SetBufferLength(size_t length) noexcept;
        [[nodiscard]] size_t GetBufferLength() const noexcept;
        bool IsBufferFull() noexcept;
        bool IsBufferEmpty() noexcept;
        void SetOnReadCallback(const std::function<void(AudioStream&, uint32_t, void*)>& callback, void* userData) noexcept;

    private:
        [[nodiscard]] double GetProgress() const noexcept override { return 0; }
        uint32_t ReadFrames(float* output, uint32_t frameCount) override;

    private:
        std::function<void(AudioStream&, uint32_t sampleCount, void* userData)> m_callback;
        void* m_userData = nullptr;
        size_t m_bufferLength = 8192;
        std::deque<float> m_buffer;
        std::mutex m_mutex;
    };
}