#include "../../include/Audio/Audio.h"
#include "../internal/Assert.h"

#define DR_FLAC_IMPLEMENTATION
#include "../../external-headers/miniaudio/decoders/dr_flac.h"  /* Enables FLAC decoding. */
#define DR_MP3_IMPLEMENTATION
#include "../../external-headers/miniaudio/decoders/dr_mp3.h"   /* Enables MP3 decoding. */
#define DR_WAV_IMPLEMENTATION
#include "../../external-headers/miniaudio/decoders/dr_wav.h"   /* Enables WAV decoding. */

#define MINIAUDIO_IMPLEMENTATION
#include "../../external-headers/miniaudio/miniaudio.h"

#include <cstring>
#include <fstream>

ma_device OUTPUT_DEVICE;
ma_device INPUT_DEVICE;

static bool DEVICES_INITIALIZED = false;

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }

    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount);

    (void)pInput;
}

void stop_callback(ma_device* pDevice)
{
}

static void Initialize_Device()
{
    if(DEVICES_INITIALIZED) return;
    ma_device_config config     = ma_device_config_init(ma_device_type_playback);
    config.playback.format      = ma_format_f32;
    config.playback.channels    = 2;
    config.sampleRate           = 48000;
    config.dataCallback         = data_callback;
    config.stopCallback         = stop_callback;
    config.pUserData            = nullptr;
    TML_ASSERT(ma_device_init(NULL, &config, &OUTPUT_DEVICE) == MA_SUCCESS, "Failed to initialize audio output device");
    ma_device_start(&OUTPUT_DEVICE);
    DEVICES_INITIALIZED = true;
}

namespace tml {
    Sound::Sound() {
        Initialize_Device();
    }

    Sound::Sound(const std::string &filename) {
        Initialize_Device();
        m_decoder = new ma_decoder;
        ma_result result = ma_decoder_init_file(filename.c_str(), nullptr, (ma_decoder*)m_decoder);
        if (result != MA_SUCCESS)
            tml::Logger::ErrorMessage("Failed to load sound file -> %s", filename.c_str());
    }

    Sound::~Sound() {
        ma_decoder_uninit((ma_decoder*)m_decoder);
        delete (ma_decoder*)m_decoder;
    }

    void Sound::Play() {
        m_state = Playing;
        OUTPUT_DEVICE.pUserData = m_decoder;
    }

    void Sound::Stop() {
        m_state = Stopped;
        ma_decoder_seek_to_pcm_frame((ma_decoder*)m_decoder, 0);
        OUTPUT_DEVICE.pUserData = nullptr;
    }

    void Sound::Pause() {
        OUTPUT_DEVICE.pUserData = nullptr;
    }

    void Sound::Resume() {
        OUTPUT_DEVICE.pUserData = m_decoder;
    }

    void Sound::SetLooping(bool loop) {
        m_looping = loop;
    }

    void Sound::SetPitch(float pitch) {

    }

    void Sound::SetVolume(float volume) {
    }
};