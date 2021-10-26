#pragma once
#include "AudioType.h"

namespace tml
{
    class Sound : public AudioType
    {
    public:
        Sound();
        Sound(const std::string& filename);
        Sound(const float* data, ui8 channels, ui64 sampleCount); // @brief Constructs sound from raw file data.
        ~Sound();

        bool LoadFromFile(const std::string& filename);
        bool LoadFromData(const void* data, ui64 bytes); // @brief Loads sound from raw file data.
        bool LoadFromMemory(const float* data, ui8 channels, ui64 sampleCount);
        void Play() override;
        void Stop() override;
    private:
        ui32 ReadFrames(float* output, ui32 frameCount) override;
        float* m_samples;
        ui64 m_readSamples;
        ui64 m_samplesCount;
        ui8 m_channels;
    };
}