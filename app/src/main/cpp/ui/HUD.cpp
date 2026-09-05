#include "ui/HUD.h"
#include "core/Log.h"
#include <cstdio>

HUD::HUD() = default;

void HUD::render(UIRenderer* ui, Font* font, int screenWidth, int screenHeight,
                const Car* playerCar, const LapManager* lapManager, const RaceManager* raceManager,
                bool isPracticeMode) {
    if (!ui || !font) return;

    // Speedometer (Top-Left)
    if (playerCar) {
        char speedStr[32];
        std::snprintf(speedStr, sizeof(speedStr), "%3.0f KM/H", playerCar->getSpeedKmh());
        ui->drawRect(15.0f, 15.0f, 180.0f, 40.0f, 0.0f, 0.0f, 0.0f, 0.5f);
        font->drawText(*ui, speedStr, 25.0f, 25.0f, 1.2f, 1.0f, 0.9f, 0.2f, 1.0f);
    }

    // Lap Info (Top-Right)
    if (lapManager) {
        int currentLap = lapManager->getLapCount(0) + 1;
        float lapTime = lapManager->getCurrentLapTime(0);

        char lapStr[32];
        std::snprintf(lapStr, sizeof(lapStr), "LAP %d", currentLap);
        ui->drawRect(screenWidth - 200.0f, 15.0f, 185.0f, 40.0f, 0.0f, 0.0f, 0.0f, 0.5f);
        font->drawText(*ui, lapStr, screenWidth - 190.0f, 25.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

        char timeStr[32];
        std::snprintf(timeStr, sizeof(timeStr), "TIME: %.2f", lapTime);
        ui->drawRect(screenWidth - 200.0f, 60.0f, 185.0f, 35.0f, 0.0f, 0.0f, 0.0f, 0.5f);
        font->drawText(*ui, timeStr, screenWidth - 190.0f, 68.0f, 0.9f, 1.0f, 1.0f, 1.0f, 1.0f);
    }

    // Race Position (Top-Center)
    if (!isPracticeMode && raceManager) {
        int pos = raceManager->getPosition(0);
        char posStr[16];
        std::snprintf(posStr, sizeof(posStr), "POS: P%d", pos);
        ui->drawRect((screenWidth - 140.0f) * 0.5f, 15.0f, 140.0f, 45.0f, 0.0f, 0.0f, 0.0f, 0.5f);
        font->drawText(*ui, posStr, (screenWidth - 120.0f) * 0.5f, 25.0f, 1.3f, 1.0f, pos == 1 ? 0.9f : 1.0f, pos == 1 ? 0.2f : 1.0f, 1.0f);
    }

    // Countdown overlay
    if (raceManager && raceManager->getState() == RaceManager::State::Countdown) {
        int countdown = raceManager->getCountdown();
        char cdStr[16];
        if (countdown > 0) {
            std::snprintf(cdStr, sizeof(cdStr), "%d", countdown);
        } else {
            std::snprintf(cdStr, sizeof(cdStr), "GO!");
        }
        font->drawText(*ui, cdStr, (screenWidth - 40.0f) * 0.5f, screenHeight * 0.4f, 3.0f, 1.0f, countdown == 0 ? 0.9f : 0.3f, countdown == 0 ? 0.2f : 0.3f, 1.0f);
    }

    // Touch controls overlay hints
    ui->drawRect(20.0f, screenHeight - 90.0f, 70.0f, 70.0f, 1.0f, 1.0f, 1.0f, 0.2f);
    font->drawText(*ui, "<", 45.0f, screenHeight - 65.0f, 1.5f, 1.0f, 1.0f, 1.0f, 1.0f);

    ui->drawRect(100.0f, screenHeight - 90.0f, 70.0f, 70.0f, 1.0f, 1.0f, 1.0f, 0.2f);
    font->drawText(*ui, ">", 125.0f, screenHeight - 65.0f, 1.5f, 1.0f, 1.0f, 1.0f, 1.0f);

    ui->drawRect(screenWidth - 180.0f, screenHeight - 90.0f, 70.0f, 70.0f, 1.0f, 1.0f, 1.0f, 0.2f);
    font->drawText(*ui, "B", screenWidth - 155.0f, screenHeight - 65.0f, 1.5f, 1.0f, 0.3f, 0.3f, 1.0f);

    ui->drawRect(screenWidth - 90.0f, screenHeight - 90.0f, 70.0f, 70.0f, 1.0f, 1.0f, 1.0f, 0.2f);
    font->drawText(*ui, "G", screenWidth - 65.0f, screenHeight - 65.0f, 1.5f, 1.0f, 0.9f, 0.2f, 1.0f);
}
