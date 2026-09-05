#include "input/InputManager.h"
#include "core/Log.h"

InputManager::InputManager() = default;

void InputManager::init(android_app* app) {
    m_app = app;
    if (m_gyroscope.init()) {
        LOGI("InputManager: Gyroscope initialized");
    }
}

void InputManager::processInput(int screenWidth, int screenHeight) {
    if (m_app) {
        m_touch.processGameActivityMotionEvents(m_app);
    }
    if (m_gyroEnabled) {
        m_gyroscope.pollEvents();
    }
}

CarInput InputManager::getCarInput(int screenWidth, int screenHeight) const {
    CarInput input = m_touch.toCarInput(screenWidth, screenHeight);

    if (m_gyroEnabled) {
        float gyroSteer = m_gyroscope.getNormalizedSteering();
        if (std::abs(gyroSteer) > 0.05f) {
            input.steering = gyroSteer; // Gyro overrides touch steering if active
        }
    }

    return input;
}

void InputManager::setGyroEnabled(bool enabled) {
    m_gyroEnabled = enabled;
    if (enabled) {
        m_gyroscope.enable();
    } else {
        m_gyroscope.disable();
    }
}

void InputManager::calibrateGyro() {
    m_gyroscope.calibrateZero();
}
