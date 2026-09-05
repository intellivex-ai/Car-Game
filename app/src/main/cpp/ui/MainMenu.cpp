#include "ui/MainMenu.h"
#include "core/Log.h"

MainMenu::MainMenu() = default;

void MainMenu::render(UIRenderer* ui, Font* font, int screenWidth, int screenHeight) {
    if (!ui || !font) return;

    // Fullscreen background
    ui->drawRect(0.0f, 0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight), 0.1f, 0.1f, 0.15f, 0.95f);

    // Title
    font->drawText(*ui, "CAR RACING GAME", (screenWidth - 320.0f) * 0.5f, 80.0f, 2.0f, 1.0f, 0.9f, 0.2f, 1.0f);

    // Buttons
    float btnW = 240.0f;
    float btnH = 50.0f;
    float centerX = (screenWidth - btnW) * 0.5f;

    ui->drawRect(centerX, 200.0f, btnW, btnH, 0.2f, 0.3f, 0.6f, 0.8f);
    font->drawText(*ui, "PRACTICE", centerX + 50.0f, 215.0f, 1.2f, 1.0f, 1.0f, 1.0f, 1.0f);

    ui->drawRect(centerX, 280.0f, btnW, btnH, 0.2f, 0.3f, 0.6f, 0.8f);
    font->drawText(*ui, "RACE MODE", centerX + 45.0f, 295.0f, 1.2f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void MainMenu::handleTouch(float touchX, float touchY, int screenWidth, int screenHeight) {
    float btnW = 240.0f;
    float btnH = 50.0f;
    float centerX = (screenWidth - btnW) * 0.5f;

    if (touchX >= centerX && touchX <= centerX + btnW && touchY >= 200.0f && touchY <= 200.0f + btnH) {
        if (m_onPractice) m_onPractice();
    } else if (touchX >= centerX && touchX <= centerX + btnW && touchY >= 280.0f && touchY <= 280.0f + btnH) {
        if (m_onRace) m_onRace();
    }
}
