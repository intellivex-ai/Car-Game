#include "gameplay/ai/AIAgent.h"
#include "core/Log.h"
#include <cmath>

void AIAgent::init(Car* car, const std::vector<JPH::Vec3>& waypoints, float skillLevel) {
    m_car = car;
    m_waypoints = waypoints;
    m_skillLevel = skillLevel;
    reset();
}

void AIAgent::reset() {
    m_currentWP = 0;
    m_stuckTimer = 0.0f;
    m_recoveryTimer = 0.0f;
    m_recovering = false;
}

CarInput AIAgent::update(float dt) {
    CarInput input;
    if (!m_car || m_waypoints.empty()) return input;

    JPH::Vec3 carPos = m_car->getPosition();
    JPH::Vec3 carForward = m_car->getForward();

    const JPH::Vec3& targetWP = m_waypoints[m_currentWP];
    JPH::Vec3 toWaypoint = targetWP - carPos;
    toWaypoint.SetY(0.0f);

    float distance = toWaypoint.Length();
    if (distance < 6.0f) {
        m_currentWP = (m_currentWP + 1) % m_waypoints.size();
    }

    if (m_recovering) {
        m_recoveryTimer -= dt;
        input.brake = 0.8f;
        input.steering = -0.5f;
        if (m_recoveryTimer <= 0.0f) {
            m_recovering = false;
        }
        return input;
    }

    toWaypoint = toWaypoint.Normalized();
    float dot = carForward.Dot(toWaypoint);
    JPH::Vec3 cross = carForward.Cross(toWaypoint);

    float angle = std::atan2(cross.GetY(), dot);
    input.steering = std::clamp(angle * 1.5f * m_skillLevel, -1.0f, 1.0f);

    float currentSpeed = m_car->getSpeedKmh();
    if (currentSpeed > 90.0f) {
        input.throttle = 0.4f;
        input.brake = 0.2f;
    } else {
        input.throttle = std::clamp(1.0f - std::abs(input.steering) * 0.3f, 0.3f, 1.0f);
        input.brake = 0.0f;
    }

    if (currentSpeed < kStuckSpeedThreshold && input.throttle > 0.5f) {
        m_stuckTimer += dt;
        if (m_stuckTimer > kStuckDuration) {
            m_recovering = true;
            m_recoveryTimer = kRecoveryDuration;
            m_stuckTimer = 0.0f;
        }
    } else {
        m_stuckTimer = 0.0f;
    }

    return input;
}
