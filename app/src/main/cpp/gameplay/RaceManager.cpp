#include "gameplay/RaceManager.h"
#include "core/Log.h"
#include <algorithm>
#include <cmath>

void RaceManager::startRace(int carCount, int totalLaps) {
    m_carCount = std::min(carCount, kMaxCars);
    m_totalLaps = totalLaps;
    m_state = State::Countdown;
    m_countdownTimer = kCountdownDuration;
    m_countdownStep = kCountdownSteps;
    m_raceTimer = 0.0f;
    m_finishedCount = 0;

    for (int i = 0; i < kMaxCars; ++i) {
        m_carStates[i].carId = i;
        m_carStates[i].position = i + 1;
        m_carStates[i].lapCount = 0;
        m_carStates[i].nextCheckpoint = 0;
        m_carStates[i].distToNext = 0.0f;
        m_carStates[i].finished = false;
    }
}

void RaceManager::update(float dt, const LapManager& lapManager,
                         const JPH::Vec3 carPositions[], const JPH::Vec3 nextWPs[], int carCount) {
    if (m_state == State::Countdown) {
        m_countdownTimer -= dt;
        if (m_countdownTimer <= 0.0f) {
            m_countdownStep--;
            m_countdownTimer = kCountdownDuration;
            if (m_countdownCallback) m_countdownCallback(m_countdownStep);

            if (m_countdownStep < 0) {
                m_state = State::Racing;
                LOGI("RaceManager: GO!");
            }
        }
    } else if (m_state == State::Racing) {
        m_raceTimer += dt;
        recalculatePositions(lapManager, carPositions, nextWPs);

        m_finishedCount = 0;
        for (int i = 0; i < m_carCount; ++i) {
            if (lapManager.hasFinished(i, m_totalLaps)) {
                if (!m_carStates[i].finished) {
                    m_carStates[i].finished = true;
                    if (m_raceFinishCallback && m_finishedCount == 0) {
                        m_raceFinishCallback(i);
                    }
                }
                m_finishedCount++;
            }
        }

        if (m_finishedCount >= m_carCount) {
            m_state = State::Finished;
            LOGI("RaceManager: All cars finished race");
        }
    }
}

void RaceManager::recalculatePositions(const LapManager& lm, const JPH::Vec3 carPos[], const JPH::Vec3 nextWPs[]) {
    for (int i = 0; i < m_carCount; ++i) {
        m_carStates[i].lapCount = lm.getLapCount(i);
        m_carStates[i].nextCheckpoint = lm.getNextCheckpoint(i);
        if (carPos && nextWPs) {
            m_carStates[i].distToNext = (nextWPs[i] - carPos[i]).Length();
        }
    }

    std::vector<int> indices(m_carCount);
    for (int i = 0; i < m_carCount; ++i) indices[i] = i;

    std::sort(indices.begin(), indices.end(), [this](int a, int b) {
        if (m_carStates[a].lapCount != m_carStates[b].lapCount)
            return m_carStates[a].lapCount > m_carStates[b].lapCount;
        if (m_carStates[a].nextCheckpoint != m_carStates[b].nextCheckpoint)
            return m_carStates[a].nextCheckpoint > m_carStates[b].nextCheckpoint;
        return m_carStates[a].distToNext < m_carStates[b].distToNext;
    });

    for (int rank = 0; rank < m_carCount; ++rank) {
        m_carStates[indices[rank]].position = rank + 1;
    }
}

int RaceManager::getPosition(int carId) const {
    if (carId >= 0 && carId < kMaxCars) return m_carStates[carId].position;
    return 1;
}

const RaceManager::CarRaceState& RaceManager::getCarState(int carId) const {
    int safeIdx = (carId >= 0 && carId < kMaxCars) ? carId : 0;
    return m_carStates[safeIdx];
}
