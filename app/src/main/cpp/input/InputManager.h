#pragma once
#include "input/TouchInput.h"
#include "input/GyroscopeInput.h"
#include "gameplay/CarInput.h"
#include <game-activity/native_app_glue/android_native_app_glue.h>

/**
 * InputManager
 *
 * Aggregates TouchInput and GyroscopeInput into a single CarInput
 * for the player car.
 *
 * Merge logic:
 *   if (gyroEnabled && gyro.isAvailable()):
 *       steering = gyro.getRollNormalized()
 *   else:
 *       steering = touch.getSteering()
 *
 *   throttle  = touch.getThrottle()
 *   brake     = touch.getBrake()
 *   handbrake = touch.getHandbrake()
 *
 * The Car Controller only talks to InputManager — it never sees TouchInput
 * or GyroscopeInput directly.
 *
 * Settings can be toggled at runtime (e.g., player toggles gyro in settings).
 */
class InputManager {
public:
    InputManager() = default;

    /**
     * Initializes touch and gyroscope subsystems.
     * @param app   android_app* for ALooper access and event draining
     */
    bool init(android_app* app);

    void destroy();

    /**
     * Polls all input devices and returns the merged player input.
     * Call once per frame before the game update.
     */
    CarInput poll(android_app* app);

    // --- Settings ---
    void setGyroscopeEnabled(bool enabled);
    bool isGyroscopeEnabled() const { return m_gyro.enabled; }

    void setGyroSensitivity(float s)   { m_gyro.sensitivity = s; }
    void setGyroDeadZone  (float deg)  { m_gyro.deadZoneDeg = deg; }
    void calibrateGyroscope()          { m_gyro.calibrate(); }

    bool isGyroscopeAvailable() const  { return m_gyro.isAvailable(); }

    // Pause detection (forwarded from touch layer)
    bool isPausePressed() const { return m_touch.getPause(); }
    void consumePause()         { m_touch.consumePause(); }

    TouchInput&    getTouch() { return m_touch; }
    GyroscopeInput& getGyro() { return m_gyro; }

private:
    TouchInput     m_touch;
    GyroscopeInput m_gyro;
};
