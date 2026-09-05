#pragma once
#include <cstdint>
#include <string>

/**
 * GameSettings
 *
 * Plain data structure representing all user preferences.
 * Stored/loaded by SaveSystem.
 *
 * Default values are applied when no save file exists.
 */
struct GameSettings {
    // Input
    int   controlMode      = 0;      // 0 = touch, 1 = gyroscope
    float gyroSensitivity  = 1.0f;
    float gyroDeadZoneDeg  = 3.0f;

    // Audio
    float masterVolume     = 1.0f;
    float sfxVolume        = 1.0f;

    // Graphics
    int   qualityPreset    = 1;      // 0=Low, 1=Medium, 2=High

    // Last session
    int   selectedCar      = 0;
    int   selectedTrack    = 0;
};
