#include "audio/AudioManager.h"
#include "core/Log.h"

AudioManager::AudioManager() = default;
AudioManager::~AudioManager() {
    shutdown();
}

bool AudioManager::init() {
    AAudioStreamBuilder* builder = nullptr;
    aaudio_result_t result = AAudio_createStreamBuilder(&builder);
    if (result != AAUDIO_OK) {
        LOGE("AudioManager: Failed to create AAudio stream builder (%d)", result);
        return false;
    }

    AAudioStreamBuilder_setFormat(builder, AAUDIO_FORMAT_PCM_I16);
    AAudioStreamBuilder_setChannelCount(builder, 2);
    AAudioStreamBuilder_setSampleRate(builder, 44100);
    AAudioStreamBuilder_setPerformanceMode(builder, AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);

    result = AAudioStreamBuilder_openStream(builder, &m_stream);
    AAudioStreamBuilder_delete(builder);

    if (result != AAUDIO_OK) {
        LOGE("AudioManager: Failed to open AAudio stream (%d)", result);
        return false;
    }

    result = AAudioStream_requestStart(m_stream);
    if (result != AAUDIO_OK) {
        LOGE("AudioManager: Failed to start AAudio stream (%d)", result);
        return false;
    }

    // Generate procedural audio effects
    m_engineSound.generateEngineSound(120.0f, 1.0f, 44100);
    m_countdownBeep.generateBeepSound(880.0f, 0.2f, 44100);

    LOGI("AudioManager: AAudio stream started successfully at 44100 Hz stereo");
    return true;
}

void AudioManager::playEngineSound(float rpmNormalized) {
    // Engine pitch modulation logic placeholder
}

void AudioManager::playCountdownBeep() {
    LOGI("AudioManager: Countdown beep!");
}

void AudioManager::shutdown() {
    if (m_stream) {
        AAudioStream_requestStop(m_stream);
        AAudioStream_close(m_stream);
        m_stream = nullptr;
    }
}
