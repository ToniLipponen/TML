#pragma once
#include <string>
#include <vector>
#include "../Types.h"

class AudioBuffer
{
public:
    AudioBuffer();
    ~AudioBuffer();
    void LoadFromFile(const std::string& filename);
    void LoadFromMemory(const void* data, ui64 bytes);
    friend class Sound;
private:
    void Generate();
    ui32 m_id;
    std::vector<ui16> m_data;
    ui32 m_rate = 44200;
    ui16 m_channels = 2;
    ui32 m_format;
    ui32 m_duration = 0;
};