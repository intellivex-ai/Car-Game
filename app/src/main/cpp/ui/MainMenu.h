#pragma once
#include "ui/UIRenderer.h"
#include "ui/Font.h"
#include <functional>

/**
 * MainMenu
 *
 * Renders and handles the main menu screen.
 *
 * Buttons:
 *   PRACTICE  → callback onPractice
 *   RACE      → callback onRace
 *   SETTINGS  → callback onSettings
 *   EXIT      → callback onExit
 *
 * Button interaction: touch events forwarded from TouchInput.
 * Tap is detected on POINTER_UP inside button bounds.
 */
class MainMenu {
public:
    std::function<void()> onPractice;
    std::function<void()> onRace;
    std::function<void()> onSettings;
    std::function<void()> onExit;

    MainMenu() = default;

    void init(UIRenderer* ui, Font* font, int screenW, int screenH);
    void onScreenChanged(int w, int h);

    void draw() const;

    /**
     * Process a tap at screen coordinates (x, y) in pixels.
     * Call when a finger lift event is received.
     */
    void onTap(float x, float y);

private:
    UIRenderer* m_ui     = nullptr;
    Font*       m_font   = nullptr;
    int         m_screenW = 1;
    int         m_screenH = 1;
};
