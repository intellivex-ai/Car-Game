#pragma once
#include "scene/IScene.h"
#include "scene/SceneManager.h"
#include "ui/UIRenderer.h"
#include "ui/Font.h"
#include "ui/MainMenu.h"
#include "renderer/IRenderer.h"

/**
 * MainMenuScene
 *
 * The initial scene shown at launch.
 * Renders the main menu and handles navigation to Practice or Race.
 *
 * On Practice/Race button press: replaces the scene stack with a new GameScene.
 */
class MainMenuScene : public IScene {
public:
    /**
     * @param sceneManager  Shared scene manager (for navigation)
     * @param renderer      Shared renderer
     * @param ui            Shared UI renderer
     * @param font          Shared font
     */
    MainMenuScene(SceneManager* sceneManager,
                  IRenderer*    renderer,
                  UIRenderer*   ui,
                  Font*         font);

    void onEnter() override;
    void onExit()  override;
    void update(float dt) override;
    void render()         override;

private:
    SceneManager* m_sceneManager = nullptr;
    IRenderer*    m_renderer     = nullptr;
    MainMenu      m_menu;

    void navigateToPractice();
    void navigateToRace();
};
