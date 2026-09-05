#pragma once
#include "gameplay/LapManager.h"
#include <functional>
#include <array>
#include <cstdint>

/**
 * RaceManager
 *
 * Manages the Race Mode state machine and calculates race positions.
 *
 * State machine:
 *
 *   IDLE      — before race starts (sitting at main menu)
 *   LOADING   — track and cars being created
 *   COUNTDOWN — 3, 2, 1, GO! (kCountdownDuration seconds each)
 *   RACING    — race in progress
 *   FINISHED  — at least one car has completed all laps
 *
 * During COUNTDOWN, cars cannot apply throttle (the CarController
 * ignores throttle input when RaceManager::canDrive() returns false).
 *
 * Position ranking:
 *   Sorted by: lapCount DESC → nextCheckpoint DESC → distToNextCheckpoint ASC
 *   This gives a position that reflects who is furthest around the track,
 *   not just who is physically closest to the finish line.
 */
class RaceManager {
public:
    static constexpr int   kMaxCars           = 8;
    static constexpr float kCountdownDuration = 1.0f;   // seconds per count
    static constexpr int   kCountdownSteps    = 3;       // "3, 2, 1, GO!"

    enum class State { Idle, Loading, Countdown, Racing, Finished };

    // Fired when the countdown tick changes (for audio/HUD)
    using CountdownCallback  = std::function<void(int count)>;       // count: 3,2,1,0=GO
    // Fired when the first car crosses the finish
    using RaceFinishCallback = std::function<void(int winnerCarId)>;

    struct CarRaceState {
        int   carId           = -1;
        int   position        = 0;    // 1-based race position
        int   lapCount        = 0;
        int   nextCheckpoint  = 0;
        float distToNext      = 0.0f;
        bool  finished        = false;
    };

    RaceManager() = default;

    /**
     * Called when a new race is started.
     * @param carCount   Total cars (player + AI)
     * @param totalLaps  Number of laps to complete
     */
    void startRace(int carCount, int totalLaps);

    /**
     * Update state machine and recalculate positions.
     * @param dt           Fixed timestep
     * @param lapManager   Shared LapManager to query per-car lap state
     * @param carPositions World positions of each car (for dist-to-next-checkpoint)
     * @param nextWPs      Next checkpoint world positions for each car
     */
    void update(float dt,
                const LapManager& lapManager,
                const JPH::Vec3 carPositions[],
                const JPH::Vec3 nextWPs[],
                int carCount);

    void setCountdownCallback (CountdownCallback  cb) { m_countdownCallback  = std::move(cb); }
    void setRaceFinishCallback(RaceFinishCallback cb) { m_raceFinishCallback = std::move(cb); }

    // --- Queries ---
    State getState()               const { return m_state; }
    bool  canDrive()               const { return m_state == State::Racing || m_state == State::Finished; }
    bool  isRacing()               const { return m_state == State::Racing; }
    bool  isFinished()             const { return m_state == State::Finished; }
    int   getCountdown()           const { return m_countdownStep; }
    int   getPosition(int carId)   const;
    const CarRaceState& getCarState(int carId) const;
    float getRaceTime()            const { return m_raceTimer; }

private:
    State     m_state        = State::Idle;
    float     m_countdownTimer = 0.0f;
    int       m_countdownStep  = kCountdownSteps;
    float     m_raceTimer      = 0.0f;
    int       m_totalLaps      = 0;
    int       m_carCount       = 0;
    int       m_finishedCount  = 0;

    CarRaceState m_carStates[kMaxCars] = {};

    CountdownCallback  m_countdownCallback;
    RaceFinishCallback m_raceFinishCallback;

    void recalculatePositions(const LapManager& lm,
                              const JPH::Vec3 carPos[],
                              const JPH::Vec3 nextWPs[]);
};
