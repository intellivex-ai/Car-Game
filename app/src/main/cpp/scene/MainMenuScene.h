#pragma once
#include "scene/IScene.h"
#include "scene/SceneManager.h"
#include "ui/UIRenderer.h"
#include "ui/Font.h"
#include "ui/MainMenu.h"
#include "renderer/IRenderer.h"
#include "audio/AudioManager.h"
#include <game-activity/native_app_glue/android_native_app_glue.h>

class MainMenuScene : public IScene {
public:
    MainMenuScene(SceneManager* sceneManager,
                  IRenderer*    renderer,
                  UIRenderer*   ui,
                  Font*         font,
                  AudioManager* audio = nullptr,
                  android_app*  app = nullptr);

    void onEnter() override;
    void onExit()  override;
    void update(float dt) override;
    void render()         override;

private:
    SceneManager* m_sceneManager = nullptr;
    IRenderer*    m_renderer     = nullptr;
    UIRenderer*   m_ui           = nullptr;
    Font*         m_font         = nullptr;
    AudioManager* m_audio        = nullptr;
    android_app*  m_app          = nullptr;
    MainMenu      m_menu;

    void navigateToPractice();
    void navigateToRace();
};
