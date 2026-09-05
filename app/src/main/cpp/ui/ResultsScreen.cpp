#include "ui/ResultsScreen.h"
#include "core/Log.h"
#include <cstdio>

ResultsScreen::ResultsScreen() = default;

void ResultsScreen::render(UIRenderer* ui, Font* font, int screenWidth, int screenHeight,
                          const LapManager* lapManager, const RaceManager* raceManager) {
    if (!ui || !font) return;

    float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float yellow[4] = { 1.0f, 0.9f, 0.2f, 1.0f };
    float buttonBg[4] = { 0.2f, 0.5f, 0.3f, 0.8f };
    float darkBg[4] = { 0.05f, 0.05f, 0.1f, 0.9f };

    ui->drawQuad(0.0f, 0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight), darkBg);

    font->drawText(ui, "RACE RESULTS", (screenWidth - 240.0f) * 0.5f, 60.0f, 2.0f, yellow);

    if (raceManager && lapManager) {
        int pos = raceManager->getCarPosition(0);
        const CarLapState* state = lapManager->getCarState(0);

        char posStr[32];
        std::snprintf(posStr, sizeof(posStr), "FINAL POSITION: P%d", pos);
        font->drawText(ui, posStr, (screenWidth - 250.0f) * 0.5f, 140.0f, 1.3f, pos == 1 ? yellow : white);

        if (state) {
            char timeStr[64];
            std::snprintf(timeStr, sizeof(timeStr), "TOTAL TIME: %.2f s", state->totalRaceTime);
            font->drawText(ui, timeStr, (screenWidth - 220.0f) * 0.5f, 190.0f, 1.0f, white);

            char bestStr[64];
            std::snprintf(bestStr, sizeof(bestStr), "BEST LAP: %.2f s", state->bestLapTime < 999.0 ? state->bestLapTime : 0.0);
            font->drawText(ui, bestStr, (screenWidth - 200.0f) * 0.5f, 230.0f, 1.0f, white);
        }
    }

    float btnW = 220.0f;
    float btnH = 50.0f;
    float centerX = (screenWidth - btnW) * 0.5f;

    ui->drawQuad(centerX, 320.0f, btnW, btnH, buttonBg);
    font->drawText(ui, "CONTINUE", centerX + 55.0f, 335.0f, 1.2f, white);
}

void ResultsScreen::handleTouch(float touchX, float touchY, int screenWidth, int screenHeight) {
    float btnW = 220.0f;
    float btnH = 50.0f;
    float centerX = (screenWidth - btnW) * 0.5f;

    if (touchX >= centerX && touchX <= centerX + btnW && touchY >= 320.0f && touchY <= 320.0f + btnH) {
        if (m_onContinue) m_onContinue();
    }
}
