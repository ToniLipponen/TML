#pragma once
#include <TML/Export.h>
#include <TML/Audio/Common.h>
#include <cstdint>
#include <vector>

namespace tml
{
    class TML_API AudioType
    {
    public:
        enum State { Stopped, Playing, Paused };

    public:
        AudioType();
        virtual ~AudioType();
        virtual void Play();
        virtual void Stop();
        virtual void Pause();
        virtual void Resume();
        void SetLooping(bool loop);
        void SetVolume(float volume);
        void SetBalance(float balance);
        [[nodiscard]] float GetVolume() const;
        [[nodiscard]] State GetState() const;
        [[nodiscard]] bool IsPlaying() const;
        [[nodiscard]] bool IsLooping() const;
        virtual uint64_t GetLength() noexcept;
        virtual uint64_t GetLengthInSeconds() noexcept;
        [[nodiscard]] virtual double GetProgress() const noexcept;
        virtual uint32_t ReadFrames(AudioFrame* output, uint32_t frameCount) = 0;
    
    protected:
        uint64_t m_framesRead;
        uint64_t m_frameCount;
        uint32_t m_rate;
        uint8_t m_channels;
        float m_volume = 1.f;
        float m_balance = 0;
        bool m_looping = false;
        bool m_valid = false;
        State m_state = Stopped;
        uint64_t m_id;
    };
}
