#include "input/GyroscopeInput.h"
#include "core/Log.h"
#include <algorithm>
#include <cmath>

GyroscopeInput::GyroscopeInput() = default;

GyroscopeInput::~GyroscopeInput() {
    disable();
}

bool GyroscopeInput::init() {
    m_sensorManager = ASensorManager_getInstanceForPackage(nullptr);
    if (!m_sensorManager) {
        LOGE("GyroscopeInput: ASensorManager not available");
        return false;
    }

    m_rotationSensor = ASensorManager_getDefaultSensor(m_sensorManager, ASENSOR_TYPE_GAME_ROTATION_VECTOR);
    if (!m_rotationSensor) {
        // Fallback to standard rotation vector
        m_rotationSensor = ASensorManager_getDefaultSensor(m_sensorManager, ASENSOR_TYPE_ROTATION_VECTOR);
    }

    if (!m_rotationSensor) {
        LOGW("GyroscopeInput: Rotation vector sensor not available on this device");
        return false;
    }

    LOGI("GyroscopeInput: Rotation sensor initialized successfully");
    return true;
}

void GyroscopeInput::enable() {
    if (!m_rotationSensor || m_enabled) return;
    ALooper* looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    m_sensorEventQueue = ASensorManager_createEventQueue(m_sensorManager, looper, 1, nullptr, nullptr);
    ASensorEventQueue_enableSensor(m_sensorEventQueue, m_rotationSensor);
    ASensorEventQueue_setEventRate(m_sensorEventQueue, m_rotationSensor, 10000); // 100Hz
    m_enabled = true;
    LOGI("GyroscopeInput: Sensor enabled");
}

void GyroscopeInput::disable() {
    if (!m_enabled) return;
    if (m_sensorEventQueue && m_rotationSensor) {
        ASensorEventQueue_disableSensor(m_sensorEventQueue, m_rotationSensor);
        ASensorManager_destroyEventQueue(m_sensorManager, m_sensorEventQueue);
        m_sensorEventQueue = nullptr;
    }
    m_enabled = false;
    LOGI("GyroscopeInput: Sensor disabled");
}

void GyroscopeInput::pollEvents() {
    if (!m_enabled || !m_sensorEventQueue) return;

    ASensorEvent event;
    while (ASensorEventQueue_getEvents(m_sensorEventQueue, &event, 1) > 0) {
        if (event.type == ASENSOR_TYPE_GAME_ROTATION_VECTOR || event.type == ASENSOR_TYPE_ROTATION_VECTOR) {
            // event.data[0..2] = x, y, z rotation components; event.data[3] = w component
            float x = event.data[0];
            float y = event.data[1];
            float z = event.data[2];
            float w = event.data[3];

            // Convert quaternion to roll angle around Z axis (steering angle)
            float roll = std::atan2(2.0f * (w * z + x * y), 1.0f - 2.0f * (y * y + z * z));
            m_filteredRoll = m_filteredRoll * 0.8f + roll * 0.2f; // Low-pass filter
        }
    }
}

float GyroscopeInput::getNormalizedSteering() const {
    if (!m_enabled) return 0.0f;

    float roll = m_filteredRoll - m_calibratedZeroRoll;
    if (std::abs(roll) < m_deadZone) return 0.0f;

    float steer = roll * m_sensitivity;
    return std::clamp(steer, -1.0f, 1.0f);
}

void GyroscopeInput::calibrateZero() {
    m_calibratedZeroRoll = m_filteredRoll;
    LOGI("GyroscopeInput: Calibrated center roll to %.3f rad", m_calibratedZeroRoll);
}
