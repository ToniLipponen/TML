#pragma once
#include "AudioType.h"
#include "AudioBuffer.h"
#include <TML/System/String.h>

namespace tml
{
    class Sound : public AudioType
    {
    public:
        Sound() = default;
        explicit Sound(const String& filename);                                         //!< Loads sound from file.
        Sound(const float* data, ui8 channels, uint32_t rate, ui64 sampleCount);        //!< Loads sound from raw file data.
        Sound(const Sound& sound);
        Sound(Sound&& sound) noexcept;
        ~Sound();

        Sound& operator=(const Sound& sound) noexcept;

        bool LoadFromFile(const String& filename) noexcept;                              //!< Loads sound from file.
        bool LoadFromData(const void* data, ui64 bytes) noexcept;                        //!< Loads sound from raw file data.
        bool LoadFromMemory(const float* data, ui8 channels,                             //!< Loads sound from samples in memory.
                            uint32_t rate, ui64 sampleCount) noexcept;
        void SetBuffer(const AudioBuffer& buffer) noexcept;
        void SetBuffer(const std::shared_ptr<AudioBuffer>& buffer) noexcept;
    private:
        ui32 ReadFrames(float* output, ui32 frameCount) override;
        std::shared_ptr<AudioBuffer> m_buffer;
    };
}