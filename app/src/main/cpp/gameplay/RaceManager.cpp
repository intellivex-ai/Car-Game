#include "gameplay/RaceManager.h"
#include "core/Log.h"

void RaceManager::init(LapManager* lapManager, const std::vector<Car*>& cars, const Track* track) {
    m_lapManager = lapManager;
    m_cars = cars;
    m_track = track;
    m_positions.resize(cars.size());
    m_state = RaceState::IDLE;
}

void RaceManager::startRace() {
    m_state = RaceState::COUNTDOWN;
    m_countdownTimer = 3.0f;
    m_countdownDigit = 3;
    if (m_lapManager) m_lapManager->reset();
    LOGI("RaceManager: Race starting — countdown 3...");
}

void RaceManager::update(float dt) {
    if (m_state == RaceState::COUNTDOWN) {
        m_countdownTimer -= dt;
        int nextDigit = std::clamp(static_cast<int>(std::ceil(m_countdownTimer)), 0, 3);
        if (nextDigit != m_countdownDigit) {
            m_countdownDigit = nextDigit;
            if (m_onCountdownTick) m_onCountdownTick(m_countdownDigit);
        }

        if (m_countdownTimer <= 0.0f) {
            m_state = RaceState::RACING;
            LOGI("RaceManager: GO!");
            if (m_onCountdownTick) m_onCountdownTick(0); // 0 = GO!
        }
    } else if (m_state == RaceState::RACING) {
        if (m_lapManager) {
            m_lapManager->update(static_cast<double>(dt));
        }
        calculatePositions();

        // Check if all cars finished
        bool allFinished = true;
        for (size_t i = 0; i < m_cars.size(); ++i) {
            auto state = m_lapManager ? m_lapManager->getCarState(i) : nullptr;
            if (state && !state->isFinished) {
                allFinished = false;
                break;
            }
        }

        if (allFinished) {
            m_state = RaceState::FINISHED;
            LOGI("RaceManager: Race finished for all cars!");
            if (m_onRaceFinish) m_onRaceFinish();
        }
    }
}

void RaceManager::calculatePositions() {
    // Sort car indices by (lap descending, nextCheckpoint descending, distance to next checkpoint ascending)
    std::vector<size_t> indices(m_cars.size());
    for (size_t i = 0; i < indices.size(); ++i) indices[i] = i;

    std::sort(indices.begin(), indices.end(), [this](size_t a, size_t b) {
        auto stateA = m_lapManager ? m_lapManager->getCarState(a) : nullptr;
        auto stateB = m_lapManager ? m_lapManager->getCarState(b) : nullptr;

        if (!stateA || !stateB) return a < b;

        if (stateA->currentLap != stateB->currentLap) {
            return stateA->currentLap > stateB->currentLap;
        }

        if (stateA->nextCheckpoint != stateB->nextCheckpoint) {
            return stateA->nextCheckpoint > stateB->nextCheckpoint;
        }

        // Compare distance to next checkpoint
        if (m_track && m_cars[a] && m_cars[b]) {
            Checkpoint* cpA = const_cast<Track*>(m_track)->getCheckpoint(stateA->nextCheckpoint);
            if (cpA) {
                float distA = (m_cars[a]->getPosition() - cpA->getPosition()).LengthSq();
                float distB = (m_cars[b]->getPosition() - cpA->getPosition()).LengthSq();
                return distA < distB;
            }
        }

        return a < b;
    });

    for (size_t rank = 0; rank < indices.size(); ++rank) {
        m_positions[indices[rank]] = static_cast<int>(rank + 1);
    }
}

int RaceManager::getCarPosition(size_t carIndex) const {
    if (carIndex < m_positions.size()) return m_positions[carIndex];
    return 1;
}
