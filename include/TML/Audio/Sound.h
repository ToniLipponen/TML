#pragma once
#include "AudioType.h"

namespace tml
{
    class Sound : public AudioType
    {
    public:
        Sound() = default;
        Sound(const std::string& filename);                                     /// @brief Loads sound from file.
        Sound(const float* data, ui8 channels, ui64 sampleCount);               /// @brief Loads sound from raw file data.
        Sound(const Sound& sound);
        Sound(Sound&& sound);
        ~Sound();

        Sound& operator=(const Sound& sound) noexcept;
        Sound& operator=(Sound&& sound) noexcept;
        bool LoadFromFile(const std::string& filename) noexcept;                         /// @brief Loads sound from file.
        bool LoadFromData(const void* data, ui64 bytes) noexcept;                        /// @brief Loads sound from raw file data.
        bool LoadFromMemory(const float* data, ui8 channels, ui64 sampleCount) noexcept; /// @brief Loads sound from samples in memory.
    private:
        ui32 ReadFrames(float* output, ui32 frameCount) override;
        void* m_decoder = nullptr;
        float* m_samples = nullptr;
    };
}