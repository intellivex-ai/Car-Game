#pragma once
#include "gameplay/ai/Waypoint.h"
#include "gameplay/CarInput.h"
#include "gameplay/Car.h"
#include <Jolt/Jolt.h>
#include <Jolt/Math/Vec3.h>
#include <vector>
#include <cstdint>

/**
 * AIAgent
 *
 * Waypoint-following AI controller for one non-player car.
 *
 * Each fixed timestep, the agent:
 *   1. Finds the vector from the car to the next waypoint.
 *   2. Computes the signed angle between the car's forward and that vector.
 *   3. Maps the angle to a steering value in [-1, 1].
 *   4. Checks remaining distance to the waypoint:
 *        - If the car's speed exceeds the waypoint's maxSpeedKmh, brake.
 *        - Otherwise, apply full throttle.
 *   5. When within the waypoint's radius, advance to the next waypoint (looping).
 *   6. Recovery: if speed < kStuckSpeedThreshold for kStuckDuration seconds,
 *      reverse briefly then resume forward.
 *
 * The agent produces a CarInput each tick, which is fed to CarController::update().
 * The AI uses the same CarController and VehiclePhysics as the player — the only
 * difference is where the CarInput comes from.
 */
class AIAgent {
public:
    static constexpr float kStuckSpeedThreshold = 1.0f;   // km/h
    static constexpr float kStuckDuration       = 3.0f;   // seconds before recovery
    static constexpr float kRecoveryDuration    = 1.5f;   // seconds of reverse

    AIAgent() = default;

    /**
     * @param car       The AI car this agent controls
     * @param waypoints Reference to the track's waypoint list (not owned)
     */
    void init(Car* car, const std::vector<Waypoint>& waypoints);

    /**
     * Computes and returns the CarInput for this tick.
     * Call CarController::update() with this result.
     */
    CarInput update(float dt);

    /** Reset agent state (e.g., after car is spawned). */
    void reset();

    int getCurrentWaypointIndex() const { return m_currentWP; }

private:
    Car*                         m_car       = nullptr;
    const std::vector<Waypoint>* m_waypoints = nullptr;
    int                          m_currentWP = 0;

    // Recovery state
    float m_stuckTimer    = 0.0f;
    float m_recoveryTimer = 0.0f;
    bool  m_recovering    = false;
};
