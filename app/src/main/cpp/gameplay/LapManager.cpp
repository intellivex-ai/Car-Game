#include "gameplay/LapManager.h"
#include "core/Log.h"

void LapManager::init(size_t totalCars, size_t totalCheckpoints, int targetLaps) {
    m_carStates.resize(totalCars);
    m_totalCheckpoints = totalCheckpoints;
    m_targetLaps = targetLaps;
    reset();
}

void LapManager::reset() {
    for (size_t i = 0; i < m_carStates.size(); ++i) {
        auto& state = m_carStates[i];
        state.carIndex = static_cast<uint32_t>(i);
        state.currentLap = 0;
        state.nextCheckpoint = 0;
        state.currentLapTime = 0.0;
        state.lastLapTime = 0.0;
        state.bestLapTime = std::numeric_limits<double>::max();
        state.totalRaceTime = 0.0;
        state.isFinished = false;
    }
}

void LapManager::update(double dt) {
    for (auto& state : m_carStates) {
        if (!state.isFinished && state.currentLap > 0) {
            state.currentLapTime += dt;
            state.totalRaceTime += dt;
        }
    }
}

void LapManager::onCheckpointPassed(size_t carIndex, int checkpointIndex) {
    if (carIndex >= m_carStates.size()) return;
    auto& state = m_carStates[carIndex];
    if (state.isFinished) return;

    // Must hit checkpoints sequentially
    if (checkpointIndex == state.nextCheckpoint) {
        if (checkpointIndex == 0) {
            // Crossed start/finish line
            if (state.currentLap > 0) {
                // Completed lap
                state.lastLapTime = state.currentLapTime;
                if (state.lastLapTime < state.bestLapTime) {
                    state.bestLapTime = state.lastLapTime;
                }
                LOGI("Car %zu completed lap %d: %.3f s (Best: %.3f s)",
                     carIndex, state.currentLap, state.lastLapTime, state.bestLapTime);

                if (m_onLapComplete) {
                    m_onLapComplete(carIndex, state.currentLap, state.lastLapTime);
                }
            }

            state.currentLap++;
            state.currentLapTime = 0.0;

            if (state.currentLap > m_targetLaps) {
                state.isFinished = true;
                LOGI("Car %zu finished race!", carIndex);
            }
        }

        state.nextCheckpoint = (state.nextCheckpoint + 1) % m_totalCheckpoints;
    }
}

const CarLapState* LapManager::getCarState(size_t carIndex) const {
    if (carIndex < m_carStates.size()) return &m_carStates[carIndex];
    return nullptr;
}
