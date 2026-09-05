#pragma once
#include <aaudio/AAudio.h>
#include <vector>
#include <memory>
#include <functional>

/**
 * SoundEffect
 *
 * Represents a single audio clip that can be played once (one-shot)
 * or looped continuously (e.g., engine sound).
 *
 * Audio data is stored as a buffer of normalized float32 mono samples at 44100 Hz.
 * The AudioManager's AAudio callback mixes all active SoundEffects into
 * the output stream each callback.
 *
 * For engine sounds: the playback rate is adjusted to simulate RPM pitch variation.
 */
class SoundEffect {
public:
    enum class Type { OneShot, Looping };

    /**
     * Create a procedurally synthesized sawtooth tone (for engine/SFX).
     * @param frequencyHz  Base pitch
     * @param durationSec  Duration (for OneShot; Looping ignores this)
     * @param type         OneShot or Looping
     */
    static SoundEffect createSynthTone(float frequencyHz, float durationSec,
                                       Type type = Type::OneShot);

    /**
     * Create a click/beep suitable for UI sounds.
     * @param frequencyHz  Pitch (e.g., 880 Hz for a high beep)
     * @param durationSec  Short duration (e.g., 0.08s)
     */
    static SoundEffect createBeep(float frequencyHz, float durationSec = 0.08f);

    void play();
    void stop();
    bool isPlaying() const { return m_playing; }

    /**
     * For looping engine sounds: set a pitch multiplier relative to the base.
     * At idle (low RPM): multiplier ≈ 0.5
     * At redline (high RPM): multiplier ≈ 2.0
     */
    void setPitchMultiplier(float pitch) { m_pitchMultiplier = pitch; }

    /**
     * Called by AudioManager's callback to fill 'numFrames' output samples.
     * Returns the number of frames that were written.
     * For OneShot: returns 0 when clip ends and sets m_playing = false.
     */
    int fillFrames(float* outputBuffer, int numFrames, int sampleRate);

private:
    Type                m_type           = Type::OneShot;
    std::vector<float>  m_samples;          // float32 mono samples at 44100 Hz
    int                 m_playhead        = 0;
    float               m_pitchMultiplier = 1.0f;
    float               m_volume          = 0.8f;
    bool                m_playing         = false;
};
