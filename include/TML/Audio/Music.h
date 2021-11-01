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
        Music(void* data, ui32 bytes); // @brief Constructs music from raw file data.
        ~Music();
        bool LoadFromFile(const std::string& filename);
        bool LoadFromData(void* data, ui32 bytes); // @brief Loads music from raw file data.
        void Stop() override;
    private:
        ui32 ReadFrames(float* output, ui32 frameCount) override;
        void* m_decoder = nullptr;
    };
}