#include "../../include/Audio/AudioBuffer.h"
#include "../../include/Audio/Wav.h"
#include "../../include/Assert.h"

#include <AL/al.h>
#include <AL/alc.h>

#include <fstream>


AudioBuffer::AudioBuffer()
{
    alGenBuffers(1, &m_id);
}

AudioBuffer::~AudioBuffer()
{
    alDeleteBuffers(1, &m_id);
}
static ui32 chunkSize = 0;
void AudioBuffer::LoadFromFile(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary);
    if(!file.is_open())
    {
        tl::Logger::ErrorMessage("Failed to load wav file -> %s", filename.c_str());
        return;
    }
    Wav_header header;
    file.read(reinterpret_cast<char*>(&header), sizeof(Wav_header));
    m_channels = header.channelsCount;
    m_rate = header.sampleRate;
    chunkSize = header.chunkSize;
    m_data.reserve(header.subChunk2Size);
    file.read(reinterpret_cast<char*>(m_data.data()), header.subChunk2Size);
    file.close();
    if(header.bitsPerSample == 8)
    {
        if(header.channelsCount == 2)
            m_format = AL_FORMAT_STEREO8;
        else
            m_format = AL_FORMAT_MONO8;
    }
    else if(header.bitsPerSample == 16)
    {
        if(header.channelsCount == 2)
            m_format = AL_FORMAT_STEREO16;
        else
            m_format = AL_FORMAT_MONO16;
    }

    Generate();
}

void AudioBuffer::Generate()
{
    alBufferData(m_id, m_format, m_data.data(), chunkSize, m_rate);
    m_data.clear();
}