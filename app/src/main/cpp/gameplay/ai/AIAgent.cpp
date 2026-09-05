#include "gameplay/ai/AIAgent.h"
#include "core/Log.h"
#include <cmath>

void AIAgent::init(Car* car, const std::vector<Waypoint>& waypoints, float skillLevel) {
    m_car = car;
    m_waypoints = waypoints;
    m_skillLevel = std::clamp(skillLevel, 0.0f, 1.0f);
    m_currentWaypointIndex = 0;
    m_stuckTimer = 0.0f;
    m_isReversing = false;
}

CarInput AIAgent::update(float dt) {
    CarInput input;
    if (!m_car || m_waypoints.empty()) return input;

    JPH::Vec3 carPos = m_car->getPosition();
    JPH::Vec3 carForward = m_car->getForwardVector();

    // Check target waypoint
    const Waypoint& wp = m_waypoints[m_currentWaypointIndex];
    JPH::Vec3 toWaypoint = wp.position - carPos;
    toWaypoint.SetY(0.0f); // ignore vertical distance

    float distance = toWaypoint.Length();
    if (distance < wp.arrivalRadius) {
        // Advance to next waypoint
        m_currentWaypointIndex = (m_currentWaypointIndex + 1) % m_waypoints.size();
    }

    if (m_isReversing) {
        m_stuckTimer -= dt;
        input.brake = 0.8f; // Reverse throttle
        input.steering = -0.5f;
        if (m_stuckTimer <= 0.0f) {
            m_isReversing = false;
        }
        return input;
    }

    toWaypoint = toWaypoint.Normalized();
    float dot = carForward.Dot(toWaypoint);
    JPH::Vec3 cross = carForward.Cross(toWaypoint);

    // Steering calculation
    float angle = std::atan2(cross.GetY(), dot);
    input.steering = std::clamp(angle * 1.5f * m_skillLevel, -1.0f, 1.0f);

    // Speed control
    float currentSpeed = m_car->getSpeedKmh();
    float speedRatio = currentSpeed / wp.maxSpeedKmh;

    if (speedRatio > 1.0f) {
        input.brake = std::clamp((speedRatio - 1.0f) * 2.0f, 0.0f, 1.0f);
        input.throttle = 0.0f;
    } else {
        input.throttle = std::clamp(1.0f - std::abs(input.steering) * 0.3f, 0.3f, 1.0f);
        input.brake = 0.0f;
    }

    // Stuck recovery check
    if (currentSpeed < 5.0f && input.throttle > 0.5f) {
        m_stuckTimer += dt;
        if (m_stuckTimer > 2.5f) {
            m_isReversing = true;
            m_stuckTimer = 1.5f; // reverse for 1.5 seconds
        }
    } else {
        m_stuckTimer = 0.0f;
    }

    return input;
}
