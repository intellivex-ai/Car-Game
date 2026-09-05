// CarController.cpp
#include "gameplay/CarController.h"
#include <algorithm>
#include <cmath>

void CarController::init(VehiclePhysics* vehicle) {
    m_vehicle = vehicle;
    m_currentSteering = 0.0f;
}

void CarController::update(const CarInput& input, float dt) {
    if (!m_vehicle) return;

    // Smooth steering toward target to reduce abrupt direction changes.
    float targetSteering = std::clamp(input.steering, -1.0f, 1.0f);
    m_currentSteering += (targetSteering - m_currentSteering) * steeringSmoothingAlpha;

    float throttle = std::clamp(input.throttle, 0.0f, 1.0f);
    float brake    = std::clamp(input.brake,    0.0f, 1.0f);

    // Speed limiter (used to even out AI performance).
    if (maxSpeedKmh > 0.0f && m_vehicle->getSpeedKmh() >= maxSpeedKmh) {
        throttle = 0.0f;
    }

    m_vehicle->apply(m_currentSteering, throttle, brake, input.handbrake, dt);
}

void CarController::reset() {
    m_currentSteering = 0.0f;
}
