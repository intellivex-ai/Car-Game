#include "gameplay/LapManager.h"
#include "core/Log.h"

void LapManager::reset(int totalCars, int checkpointCount, int totalLaps) {
    m_totalCars = std::min(totalCars, kMaxCars);
    m_checkpointCount = checkpointCount;
    m_totalLaps = totalLaps;

    for (int i = 0; i < kMaxCars; ++i) {
        m_cars[i].lapCount = 0;
        m_cars[i].nextCheckpoint = 0;
        m_cars[i].lapTimer = 0.0f;
        m_cars[i].lastLapTime = kInvalidTime;
        m_cars[i].bestLapTime = kInvalidTime;
        m_cars[i].finished = false;
    }
}

void LapManager::onCheckpointPassed(int carId, int checkpointIndex) {
    if (carId < 0 || carId >= m_totalCars) return;
    auto& state = m_cars[carId];
    if (state.finished) return;

    if (checkpointIndex == state.nextCheckpoint) {
        if (checkpointIndex == 0) {
            if (state.lapCount > 0) {
                state.lastLapTime = state.lapTimer;
                bool isBest = false;
                if (state.lastLapTime < state.bestLapTime) {
                    state.bestLapTime = state.lastLapTime;
                    isBest = true;
                }
                LOGI("Car %d completed lap %d: %.3f s", carId, state.lapCount, state.lastLapTime);
                if (m_lapCompleteCallback) {
                    m_lapCompleteCallback(carId, state.lastLapTime, isBest);
                }
            }
            state.lapCount++;
            state.lapTimer = 0.0f;

            if (m_totalLaps > 0 && state.lapCount > m_totalLaps) {
                state.finished = true;
            }
        }
        state.nextCheckpoint = (state.nextCheckpoint + 1) % m_checkpointCount;
    }
}

void LapManager::update(float dt) {
    for (int i = 0; i < m_totalCars; ++i) {
        if (!m_cars[i].finished && m_cars[i].lapCount > 0) {
            m_cars[i].lapTimer += dt;
        }
    }
}

int LapManager::getLapCount(int carId) const {
    if (carId >= 0 && carId < kMaxCars) return m_cars[carId].lapCount;
    return 0;
}

int LapManager::getNextCheckpoint(int carId) const {
    if (carId >= 0 && carId < kMaxCars) return m_cars[carId].nextCheckpoint;
    return 0;
}

float LapManager::getCurrentLapTime(int carId) const {
    if (carId >= 0 && carId < kMaxCars) return m_cars[carId].lapTimer;
    return 0.0f;
}

float LapManager::getBestLapTime(int carId) const {
    if (carId >= 0 && carId < kMaxCars) return m_cars[carId].bestLapTime;
    return kInvalidTime;
}

float LapManager::getLastLapTime(int carId) const {
    if (carId >= 0 && carId < kMaxCars) return m_cars[carId].lastLapTime;
    return kInvalidTime;
}

bool LapManager::hasFinished(int carId, int totalLaps) const {
    if (carId >= 0 && carId < kMaxCars) return m_cars[carId].finished || m_cars[carId].lapCount > totalLaps;
    return false;
}
