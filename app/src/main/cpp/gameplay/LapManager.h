#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Math/Vec3.h>
#include <array>
#include <limits>
#include <functional>
#include <cstdint>

/**
 * LapManager
 *
 * Tracks lap progress and timing for every car in the race.
 * One LapManager instance is shared by all cars.
 *
 * Lap validity rule:
 *   A car must pass checkpoint 0, 1, 2, ... N in order.
 *   Passing any checkpoint out of order is ignored.
 *   When checkpoint N (the last one, which is the finish line) is passed
 *   after all preceding ones, a lap is completed.
 *
 * Usage:
 *   - Call reset(totalCars, totalLaps) at race start.
 *   - Call onCheckpointPassed(carId, checkpointIndex) from the physics callback.
 *   - Poll getLapCount(carId), getCurrentLapTime(carId), getBestLapTime(carId).
 *   - setLapCompleteCallback() to receive events when a car finishes a lap.
 */
class LapManager {
public:
    static constexpr int   kMaxCars         = 8;
    static constexpr int   kMaxCheckpoints  = 32;
    static constexpr float kInvalidTime     = std::numeric_limits<float>::infinity();

    using LapCompleteCallback = std::function<void(int carId, float lapTime, bool isBestLap)>;

    LapManager() = default;

    /** Called once at race/practice start. */
    void reset(int totalCars, int checkpointCount, int totalLaps = 0);

    /** Called from PhysicsSystem contact listener when a car body enters a checkpoint sensor. */
    void onCheckpointPassed(int carId, int checkpointIndex);

    /** Advance timers by dt. Call once per fixed timestep. */
    void update(float dt);

    void setLapCompleteCallback(LapCompleteCallback cb) { m_lapCompleteCallback = std::move(cb); }

    // --- Per-car queries ---
    int   getLapCount         (int carId) const;
    int   getNextCheckpoint   (int carId) const;
    float getCurrentLapTime   (int carId) const;
    float getBestLapTime      (int carId) const;
    float getLastLapTime      (int carId) const;
    bool  hasFinished         (int carId, int totalLaps) const;

private:
    struct CarLapState {
        int   lapCount       = 0;
        int   nextCheckpoint = 0;
        float lapTimer       = 0.0f;   // seconds into current lap
        float lastLapTime    = kInvalidTime;
        float bestLapTime    = kInvalidTime;
        bool  finished       = false;
    };

    CarLapState         m_cars[kMaxCars] = {};
    int                 m_totalCars      = 0;
    int                 m_checkpointCount= 0;
    int                 m_totalLaps      = 0;
    LapCompleteCallback m_lapCompleteCallback;
};
