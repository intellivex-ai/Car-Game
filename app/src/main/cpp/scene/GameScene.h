#pragma once
#include "scene/IScene.h"
#include "scene/SceneManager.h"
#include "physics/PhysicsSystem.h"
#include "gameplay/Car.h"
#include "gameplay/Track.h"
#include "gameplay/LapManager.h"
#include "gameplay/RaceManager.h"
#include "gameplay/ai/AIAgent.h"
#include "input/InputManager.h"
#include "renderer/IRenderer.h"
#include "renderer/Camera.h"
#include "ui/UIRenderer.h"
#include "ui/Font.h"
#include "ui/HUD.h"
#include "ui/PauseMenu.h"
#include "ui/ResultsScreen.h"
#include "audio/AudioManager.h"
#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <array>
#include <memory>

/**
 * GameScene
 *
 * The main in-game scene. Owns all race-specific systems:
 *   - PhysicsSystem (Jolt world)
 *   - Track
 *   - 4 Cars (player[0] + AI[1,2,3])
 *   - LapManager
 *   - RaceManager (nullptr in Practice mode)
 *   - 3 AIAgents
 *   - InputManager (player)
 *   - Camera
 *   - HUD / PauseMenu / ResultsScreen
 *
 * Modes:
 *   Practice — no RaceManager; unlimited laps; results show best lap only.
 *   Race     — full RaceManager with countdown; results show positions.
 */
class GameScene : public IScene {
public:
    enum class Mode { Practice, Race };

    static constexpr int kAICount       = 3;
    static constexpr int kTotalCars     = kAICount + 1;
    static constexpr int kPlayerCarId   = 0;
    static constexpr int kTotalRaceLaps = 3;

    GameScene(Mode           mode,
              SceneManager*  sceneManager,
              IRenderer*     renderer,
              UIRenderer*    uiRenderer,
              Font*          font,
              AudioManager*  audio,
              android_app*   app);

    ~GameScene() override;

    void onEnter() override;
    void onExit()  override;
    void update(float dt) override;
    void render()         override;

private:
    Mode           m_mode;
    SceneManager*  m_sceneManager = nullptr;
    IRenderer*     m_renderer     = nullptr;
    UIRenderer*    m_uiRenderer   = nullptr;
    Font*          m_font         = nullptr;
    AudioManager*  m_audio        = nullptr;
    android_app*   m_app          = nullptr;

    PhysicsSystem  m_physics;
    Track          m_track;
    LapManager     m_lapManager;
    RaceManager    m_raceManager;
    InputManager   m_inputManager;
    Camera         m_camera;

    // Cars: index 0 = player, 1–3 = AI
    std::array<std::unique_ptr<Car>,     kTotalCars> m_cars;
    std::array<std::unique_ptr<AIAgent>, kAICount>   m_aiAgents;

    HUD           m_hud;
    PauseMenu     m_pauseMenu;
    ResultsScreen m_resultsScreen;

    bool m_paused   = false;
    bool m_finished = false;

    bool initPhysics();
    bool initCars();
    bool initTrack();
    bool initManagers();
    void setupCallbacks();

    void updateInput();
    void updateAI(float dt);
    void updateCars(const CarInput& playerInput, float dt);
    void updateCamera(float dt);

    void showResults();
    void returnToMenu();
};
