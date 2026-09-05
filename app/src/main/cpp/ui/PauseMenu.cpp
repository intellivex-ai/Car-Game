#include "ui/PauseMenu.h"
#include "core/Log.h"

PauseMenu::PauseMenu() = default;

void PauseMenu::render(UIRenderer* ui, Font* font, int screenWidth, int screenHeight) {
    if (!ui || !font) return;

    // Overlay
    ui->drawRect(0.0f, 0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight), 0.0f, 0.0f, 0.0f, 0.7f);

    // Title
    font->drawText(*ui, "PAUSED", (screenWidth - 120.0f) * 0.5f, 100.0f, 2.0f, 1.0f, 0.9f, 0.2f, 1.0f);

    float btnW = 200.0f;
    float btnH = 45.0f;
    float centerX = (screenWidth - btnW) * 0.5f;

    ui->drawRect(centerX, 200.0f, btnW, btnH, 0.3f, 0.3f, 0.4f, 0.8f);
    font->drawText(*ui, "RESUME", centerX + 50.0f, 212.0f, 1.1f, 1.0f, 1.0f, 1.0f, 1.0f);

    ui->drawRect(centerX, 270.0f, btnW, btnH, 0.3f, 0.3f, 0.4f, 0.8f);
    font->drawText(*ui, "MAIN MENU", centerX + 35.0f, 282.0f, 1.1f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void PauseMenu::handleTouch(float touchX, float touchY, int screenWidth, int screenHeight) {
    float btnW = 200.0f;
    float btnH = 45.0f;
    float centerX = (screenWidth - btnW) * 0.5f;

    if (touchX >= centerX && touchX <= centerX + btnW && touchY >= 200.0f && touchY <= 200.0f + btnH) {
        if (m_onResume) m_onResume();
    } else if (touchX >= centerX && touchX <= centerX + btnW && touchY >= 270.0f && touchY <= 270.0f + btnH) {
        if (m_onMainMenu) m_onMainMenu();
    }
}
