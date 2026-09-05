#pragma once
#include "ui/UIRenderer.h"
#include "ui/Font.h"
#include "gameplay/LapManager.h"
#include "gameplay/RaceManager.h"
#include <functional>

/**
 * ResultsScreen
 *
 * Displayed after a race or practice session ends.
 *
 * Shows:
 *   - Final race position (Race mode) or session summary (Practice)
 *   - Total race time
 *   - Best lap time
 *   - All car positions (Race mode)
 *
 * Buttons:
 *   RETRY      → onRetry
 *   MAIN MENU  → onMainMenu
 */
class ResultsScreen {
public:
    std::function<void()> onRetry;
    std::function<void()> onMainMenu;

    ResultsScreen() = default;

    void init(UIRenderer* ui, Font* font, int screenW, int screenH);
    void onScreenChanged(int w, int h);

    /**
     * Populate results before displaying.
     */
    void setResults(int playerPosition, int totalCars,
                    float totalRaceTime, float bestLapTime,
                    const LapManager& lapManager, int carCount);

    void draw() const;
    void onTap(float x, float y);

private:
    UIRenderer* m_ui     = nullptr;
    Font*       m_font   = nullptr;
    int         m_screenW = 1;
    int         m_screenH = 1;

    int   m_playerPosition = 1;
    int   m_totalCars      = 4;
    float m_totalTime      = 0.0f;
    float m_bestLap        = 0.0f;

    // Per-car best laps for the results table
    static constexpr int kMaxCars = 8;
    float m_carBestLaps[kMaxCars] = {};
    int   m_carCount = 0;
};
