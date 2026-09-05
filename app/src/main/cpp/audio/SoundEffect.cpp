#include "audio/SoundEffect.h"
#include <cmath>

SoundEffect::SoundEffect() = default;

void SoundEffect::generateEngineSound(float baseFreq, float durationSec, int sampleRate) {
    size_t numSamples = static_cast<size_t>(durationSec * sampleRate);
    m_samples.resize(numSamples);

    // Sawtooth waveform with lowpass filtering for motor rumble simulation
    float phase = 0.0f;
    float phaseIncrement = baseFreq / static_cast<float>(sampleRate);

    for (size_t i = 0; i < numSamples; ++i) {
        float sample = 2.0f * (phase - std::floor(phase + 0.5f)); // Sawtooth -1..1
        m_samples[i] = static_cast<int16_t>(sample * 8000.0f);
        phase += phaseIncrement;
        if (phase >= 1.0f) phase -= 1.0f;
    }
}

void SoundEffect::generateBeepSound(float freq, float durationSec, int sampleRate) {
    size_t numSamples = static_cast<size_t>(durationSec * sampleRate);
    m_samples.resize(numSamples);

    float phase = 0.0f;
    float phaseIncrement = (2.0f * 3.14159265f * freq) / static_cast<float>(sampleRate);

    for (size_t i = 0; i < numSamples; ++i) {
        float sample = std::sin(phase); // Pure sine wave beep
        m_samples[i] = static_cast<int16_t>(sample * 16000.0f);
        phase += phaseIncrement;
    }
}
