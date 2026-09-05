#pragma once
#include "ui/UIRenderer.h"
#include "ui/Font.h"
#include "gameplay/LapManager.h"
#include "gameplay/RaceManager.h"
#include "gameplay/Car.h"

/**
 * HUD
 *
 * Renders the in-game heads-up display overlay.
 *
 * Draws (all via UIRenderer + Font):
 *   - Current speed (km/h)
 *   - Lap counter: "LAP 2/3"
 *   - Current lap time: "1:24.52"
 *   - Best lap time (Practice mode) / Race position "P 2/4" (Race mode)
 *   - Touch control buttons (semi-transparent overlaid rects)
 *   - Pause button
 *   - Countdown display ("3", "2", "1", "GO!")
 *
 * Practice vs Race mode controlled by setMode().
 */
class HUD {
public:
    enum class Mode { Practice, Race };

    HUD() = default;

    /**
     * Must be called once after UIRenderer and Font are initialized.
     */
    void init(UIRenderer* ui, Font* font, int screenW, int screenH);
    void onScreenChanged(int w, int h);

    void setMode(Mode mode) { m_mode = mode; }

    /**
     * Draw the full HUD for this frame.
     * @param player       The player's Car (for speed)
     * @param lapManager   For lap count and times
     * @param raceManager  For countdown and position (Race mode)
     * @param playerCarId  Index of the player car
     * @param totalLaps    Total laps in this session
     */
    void draw(const Car& player,
              const LapManager& lapManager,
              const RaceManager& raceManager,
              int playerCarId,
              int totalLaps) const;

private:
    UIRenderer* m_ui     = nullptr;
    Font*       m_font   = nullptr;
    Mode        m_mode   = Mode::Practice;
    int         m_screenW = 1;
    int         m_screenH = 1;

    void drawSpeed(float kmh) const;
    void drawLapInfo(const LapManager& lm, int carId, int total) const;
    void drawTime(float seconds, float x, float y, float scale) const;
    void drawTouchButtons() const;
    void drawCountdown(int count) const;
    void drawPosition(int pos, int total) const;
};
