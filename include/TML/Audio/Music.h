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
        Music(const char* data, uint32_t bytes); //!< @brief Constructs music from raw file data. Data has to be available during playback.
        ~Music();
        bool LoadFromFile(const std::string& filename);
        bool LoadFromData(const char* data, uint32_t bytes); //!< @brief Loads music from raw file data. Data has to be available during playback.
        void Stop() override;
    private:
        uint32_t ReadFrames(float* output, uint32_t frameCount) override;
        void* m_decoder = nullptr;
    };
}