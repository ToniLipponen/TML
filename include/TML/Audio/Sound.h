#pragma once
#include "AudioType.h"

namespace tml
{
    class Sound : public AudioType
    {
    public:
        Sound() = default;
        Sound(const std::string& filename);
        Sound(const float* data, ui8 channels, ui64 sampleCount); // @brief Constructs sound from raw file data.
        ~Sound();

        Sound& operator=(const Sound& sound);
        Sound& operator=(Sound&& sound);
        bool LoadFromFile(const std::string& filename);
        bool LoadFromData(const void* data, ui64 bytes); // @brief Loads sound from raw file data.
        bool LoadFromMemory(const float* data, ui8 channels, ui64 sampleCount);
    private:
        ui32 ReadFrames(float* output, ui32 frameCount) override;
        void* m_decoder = nullptr;
        float* m_samples;
    };
}