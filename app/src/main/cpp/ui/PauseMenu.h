#pragma once
#include "ui/UIRenderer.h"
#include "ui/Font.h"
#include <functional>

/**
 * PauseMenu
 *
 * Semi-transparent overlay shown when the game is paused.
 *
 * Buttons:
 *   RESUME    → onResume
 *   RESTART   → onRestart
 *   SETTINGS  → onSettings
 *   MAIN MENU → onMainMenu
 */
class PauseMenu {
public:
    std::function<void()> onResume;
    std::function<void()> onRestart;
    std::function<void()> onSettings;
    std::function<void()> onMainMenu;

    PauseMenu() = default;

    void init(UIRenderer* ui, Font* font, int screenW, int screenH);
    void onScreenChanged(int w, int h);
    void draw() const;
    void onTap(float x, float y);

private:
    UIRenderer* m_ui     = nullptr;
    Font*       m_font   = nullptr;
    int         m_screenW = 1;
    int         m_screenH = 1;
};
