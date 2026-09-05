#pragma once
#include "audio/SoundEffect.h"
#include <aaudio/AAudio.h>
#include <vector>
#include <mutex>
#include <memory>

/**
 * AudioManager
 *
 * Manages an AAudio output stream and mixes all active SoundEffects.
 *
 * AAudio (API 26+) provides the lowest-latency audio path on Android.
 * The stream callback runs on a high-priority audio thread.
 *
 * Mixing:
 *   The callback is called with a buffer of N frames.
 *   Each active SoundEffect::fillFrames() is called and its output is
 *   accumulated into the output buffer (simple additive mixing with clipping).
 *
 * Thread safety:
 *   m_sounds is protected by m_mutex since game code can call play()/stop()
 *   from the main thread while the callback runs on the audio thread.
 *
 * Predefined sounds (created during init()):
 *   - Engine idle (looping, low pitch)
 *   - Countdown beep
 *   - Race start horn
 *   - Lap complete chime
 *   - UI button click
 */
class AudioManager {
public:
    AudioManager() = default;
    ~AudioManager();

    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    /**
     * Creates the AAudio stream and synthesizes all built-in sound effects.
     * Returns false if AAudio is unavailable.
     */
    bool init();

    /** Stops the stream and releases all resources. */
    void destroy();

    // --- Built-in sound control ---
    void setEngineRPM(float rpm);   // rpm ∈ [800, 7000]; adjusts engine pitch
    void playCountdownBeep(int count); // count: 3,2,1 = beep; 0 = horn
    void playLapComplete();
    void playButtonClick();

    /** Resume/pause audio stream when app focus changes. */
    void onFocusGained();
    void onFocusLost();

private:
    AAudioStream*               m_stream  = nullptr;
    std::vector<SoundEffect*>   m_sounds;
    std::mutex                  m_mutex;

    SoundEffect*  m_engineSound    = nullptr;
    SoundEffect*  m_beepLow        = nullptr;
    SoundEffect*  m_beepHigh       = nullptr;
    SoundEffect*  m_horn           = nullptr;
    SoundEffect*  m_lapChime       = nullptr;
    SoundEffect*  m_uiClick        = nullptr;

    static constexpr int kSampleRate   = 44100;
    static constexpr int kChannelCount = 1;      // mono; duplicated to stereo by AAudio

    static aaudio_data_callback_result_t audioCallback(
        AAudioStream* stream, void* userData,
        void* audioData, int32_t numFrames);
};
