#include "ui/ResultsScreen.h"
#include "core/Log.h"
#include <cstdio>

ResultsScreen::ResultsScreen() = default;

void ResultsScreen::render(UIRenderer* ui, Font* font, int screenWidth, int screenHeight,
                          const LapManager* lapManager, const RaceManager* raceManager) {
    if (!ui || !font) return;

    ui->drawRect(0.0f, 0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight), 0.05f, 0.05f, 0.1f, 0.9f);

    font->drawText(*ui, "RACE RESULTS", (screenWidth - 240.0f) * 0.5f, 60.0f, 2.0f, 1.0f, 0.9f, 0.2f, 1.0f);

    if (raceManager && lapManager) {
        int pos = raceManager->getPosition(0);
        float bestLap = lapManager->getBestLapTime(0);

        char posStr[32];
        std::snprintf(posStr, sizeof(posStr), "FINAL POSITION: P%d", pos);
        font->drawText(*ui, posStr, (screenWidth - 250.0f) * 0.5f, 140.0f, 1.3f, 1.0f, pos == 1 ? 0.9f : 1.0f, pos == 1 ? 0.2f : 1.0f, 1.0f);

        char bestStr[64];
        std::snprintf(bestStr, sizeof(bestStr), "BEST LAP: %.2f s", bestLap < 999.0f ? bestLap : 0.0f);
        font->drawText(*ui, bestStr, (screenWidth - 200.0f) * 0.5f, 200.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    }

    float btnW = 220.0f;
    float btnH = 50.0f;
    float centerX = (screenWidth - btnW) * 0.5f;

    ui->drawRect(centerX, 320.0f, btnW, btnH, 0.2f, 0.5f, 0.3f, 0.8f);
    font->drawText(*ui, "CONTINUE", centerX + 55.0f, 335.0f, 1.2f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void ResultsScreen::handleTouch(float touchX, float touchY, int screenWidth, int screenHeight) {
    float btnW = 220.0f;
    float btnH = 50.0f;
    float centerX = (screenWidth - btnW) * 0.5f;

    if (touchX >= centerX && touchX <= centerX + btnW && touchY >= 320.0f && touchY <= 320.0f + btnH) {
        if (m_onContinue) m_onContinue();
    }
}
