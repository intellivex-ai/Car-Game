#pragma once
#include "gameplay/ai/Waypoint.h"
#include "gameplay/CarInput.h"
#include "gameplay/Car.h"
#include <Jolt/Jolt.h>
#include <Jolt/Math/Vec3.h>
#include <vector>

class AIAgent {
public:
    static constexpr float kStuckSpeedThreshold = 1.0f;
    static constexpr float kStuckDuration       = 3.0f;
    static constexpr float kRecoveryDuration    = 1.5f;

    AIAgent() = default;

    void init(Car* car, const std::vector<JPH::Vec3>& waypoints, float skillLevel = 1.0f);
    CarInput update(float dt);
    void reset();

    int getCurrentWaypointIndex() const { return m_currentWP; }

private:
    Car*                   m_car       = nullptr;
    std::vector<JPH::Vec3> m_waypoints;
    int                    m_currentWP = 0;
    float                  m_skillLevel = 1.0f;

    float m_stuckTimer    = 0.0f;
    float m_recoveryTimer = 0.0f;
    bool  m_recovering    = false;
};
