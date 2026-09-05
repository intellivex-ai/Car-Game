#pragma once
#include <android/sensor.h>
#include <Jolt/Jolt.h>
#include <Jolt/Math/Vec3.h>
#include <Jolt/Math/Quat.h>

/**
 * GyroscopeInput
 *
 * Reads the Android GAME_ROTATION_VECTOR sensor and converts device rotation
 * into a normalized steering value.
 *
 * Sensor choice: ASENSOR_TYPE_GAME_ROTATION_VECTOR
 *   - Unlike TYPE_ROTATION_VECTOR, this does NOT use the magnetometer,
 *     so it avoids interference from car speakers or external magnets.
 *   - Provides orientation as a quaternion relative to a fixed reference frame.
 *
 * Calibration:
 *   - When the player calls calibrate(), the current quaternion is stored as
 *     the "neutral" reference.
 *   - Each frame, the relative rotation from the reference to the current
 *     orientation is computed.
 *   - The roll angle of that relative rotation is extracted and used as steering.
 *
 * Dead zone:
 *   - Roll angles within ±deadZoneDeg from zero produce zero steering.
 *   - Beyond the dead zone, the value ramps linearly to ±1 at ±maxRollDeg.
 *
 * Thread safety:
 *   - processEvents() is called from the game thread.
 *   - ASensorEventQueue delivers events synchronously on the calling thread.
 */
class GyroscopeInput {
public:
    bool  enabled      = false;   // Set by player in settings
    float sensitivity  = 1.0f;   // Multiplier applied after normalization
    float deadZoneDeg  = 3.0f;   // Roll angle treated as zero
    float maxRollDeg   = 30.0f;  // Roll angle that produces ±1 steering

    GyroscopeInput() = default;
    ~GyroscopeInput();

    /**
     * Initializes the sensor manager and subscribes to GAME_ROTATION_VECTOR.
     * @param looper  The ALooper from android_app (used by ASensorEventQueue)
     * Returns false if the sensor is unavailable on this device.
     */
    bool init(ALooper* looper);

    /** Unsubscribes and releases sensor resources. */
    void destroy();

    /**
     * Reads pending sensor events from the queue.
     * Call once per frame before querying getRollNormalized().
     */
    void processEvents();

    /**
     * Stores the current orientation as the "straight ahead" reference.
     * Call when the player taps the "Calibrate" button.
     */
    void calibrate();

    /**
     * Returns the steering value in [-1, 1] based on tilt from calibration point.
     * Returns 0 if the gyroscope is disabled or not available.
     */
    float getRollNormalized() const;

    bool isAvailable() const { return m_sensor != nullptr; }

private:
    ASensorManager*   m_sensorManager = nullptr;
    const ASensor*    m_sensor        = nullptr;
    ASensorEventQueue* m_eventQueue   = nullptr;

    JPH::Quat m_referenceQuat { JPH::Quat::sIdentity() };
    JPH::Quat m_currentQuat   { JPH::Quat::sIdentity() };

    bool m_calibrated = false;

    static constexpr int kLooperIdSensor = 42;
};
