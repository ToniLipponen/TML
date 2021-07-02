#pragma once
#include "../Types.h"

struct Wav_header
{
    // RIFF
    ui32 chunkID;
    ui32 chunkSize;
    ui32 format;

    // fmt
    ui32 subChunk1ID;
    ui32 subChunk1Size;
    ui16 audioFormat;
    ui16 channelsCount;
    ui32 sampleRate;
    ui32 byteRate;
    ui16 blockAlign;
    ui16 bitsPerSample;
    
    ui32 subChunk2ID;
    ui32 subChunk2Size;

};