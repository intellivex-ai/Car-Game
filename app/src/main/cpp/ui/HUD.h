#pragma once
#include "ui/UIRenderer.h"
#include "ui/Font.h"
#include "gameplay/LapManager.h"
#include "gameplay/RaceManager.h"
#include "gameplay/Car.h"

class HUD {
public:
    enum class Mode { Practice, Race };

    HUD() = default;

    void init(UIRenderer* ui, Font* font, int screenW, int screenH);
    void onScreenChanged(int w, int h);
    void setMode(Mode mode) { m_mode = mode; }

    void render(UIRenderer* ui, Font* font, int screenWidth, int screenHeight,
                const Car* playerCar, const LapManager* lapManager, const RaceManager* raceManager,
                bool isPracticeMode);

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
};
