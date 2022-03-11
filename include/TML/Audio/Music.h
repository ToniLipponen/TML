#pragma once
#include <string>
#include "AudioType.h"

namespace tml
{
    class Music : public AudioType
    {
    public:
        Music() = default;
        explicit Music(const std::string &filename);
        Music(const char* data, ui32 bytes); //!< @brief Constructs music from raw file data. Data has to be available during playback.
        ~Music();
        bool LoadFromFile(const std::string& filename);
        bool LoadFromData(const char* data, ui32 bytes); //!< @brief Loads music from raw file data. Data has to be available during playback.
        void Stop() override;
    private:
        ui32 ReadFrames(float* output, ui32 frameCount) override;
        void* m_decoder = nullptr;
    };
}