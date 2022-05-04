#pragma once
#include "AudioType.h"
#include "AudioBuffer.h"
#include <TML/System/String.h>
#include <TML/Export.h>

namespace tml
{
    class TML_API Sound : public AudioType
    {
    public:
        Sound() = default;
        explicit Sound(const String& filename);                                         //!< Loads sound from file.
        Sound(const void* data, size_t bytes);                                          //!< Loads sound from raw file data.
        Sound(const float* data, uint8_t channels, uint32_t rate, size_t sampleCount);  //!< Loads sound from samples in memory.
        Sound(const Sound& sound);                                                      //!< Copy constructor.
        Sound(Sound&& sound) noexcept;                                                  //!< Move constructor.
        ~Sound();                                                                       //!< Destructor.

        Sound& operator=(const Sound& sound) noexcept;
        bool LoadFromFile(const String& filename) noexcept;                              //!< Loads sound from file.
        bool LoadFromData(const void* data, size_t bytes) noexcept;                      //!< Loads sound from raw file data.
        bool LoadFromMemory(const float* data, uint8_t channels,                         //!< Loads sound from samples in memory.
                            uint32_t rate, size_t sampleCount) noexcept;
        void SetBuffer(const AudioBuffer& buffer) noexcept;
        void SetBuffer(const std::shared_ptr<AudioBuffer>& buffer) noexcept;
    private:
        uint32_t ReadFrames(float* output, uint32_t frameCount) override;
        std::shared_ptr<AudioBuffer> m_buffer;
    };
}