#include "scene/GameScene.h"
#include "scene/MainMenuScene.h"
#include "core/Log.h"

GameScene::GameScene(Mode mode, SceneManager* sceneManager, IRenderer* renderer,
                     UIRenderer* uiRenderer, Font* font, AudioManager* audio, android_app* app)
    : m_mode(mode), m_sceneManager(sceneManager), m_renderer(renderer),
      m_uiRenderer(uiRenderer), m_font(font), m_audio(audio), m_app(app) {}

GameScene::~GameScene() = default;

void GameScene::onEnter() {
    LOGI("GameScene entered");
    initPhysics();
    initTrack();
    initCars();
    initManagers();
    setupCallbacks();
}

void GameScene::onExit() {
    LOGI("GameScene exited");
    for (auto& car : m_cars) {
        if (car) car->destroy(m_physics);
    }
    m_track.destroy(m_physics);
    m_physics.shutdown();
}

bool GameScene::initPhysics() {
    return m_physics.init();
}

bool GameScene::initTrack() {
    return m_track.buildOvalTrack(m_physics);
}

bool GameScene::initCars() {
    Material redMat = Material::solidColor(0.9f, 0.2f, 0.2f);
    Material blueMat = Material::solidColor(0.2f, 0.4f, 0.9f);

    for (int i = 0; i < kTotalCars; ++i) {
        m_cars[i] = std::make_unique<Car>(i);
        JPH::Vec3 spawnPos = m_track.getSpawnPoint(i);
        m_cars[i]->init(m_physics, spawnPos, i == 0 ? redMat : blueMat);
    }

    const auto& waypoints = m_track.getAIWaypoints();
    for (int i = 0; i < kAICount; ++i) {
        m_aiAgents[i] = std::make_unique<AIAgent>();
        m_aiAgents[i]->init(m_cars[i + 1].get(), waypoints, 0.8f + i * 0.05f);
    }
    return true;
}

bool GameScene::initManagers() {
    int carCount = (m_mode == Mode::Practice) ? 1 : kTotalCars;
    m_lapManager.reset(carCount, m_track.getCheckpointCount(), kTotalRaceLaps);
    m_inputManager.init(m_app);

    if (m_mode == Mode::Race) {
        m_raceManager.startRace(carCount, kTotalRaceLaps);
    }
    return true;
}

void GameScene::setupCallbacks() {
    m_physics.setCheckpointCallback([this](int carId, int checkpointIndex) {
        m_lapManager.onCheckpointPassed(carId, checkpointIndex);
    });

    m_pauseMenu.setOnResume([this]() { m_paused = false; });
    m_pauseMenu.setOnMainMenu([this]() { returnToMenu(); });
    m_resultsScreen.setOnContinue([this]() { returnToMenu(); });
}

void GameScene::update(float dt) {
    if (m_paused) return;

    m_physics.step(dt);
    m_lapManager.update(dt);

    CarInput playerInput;
    int w = m_renderer ? m_renderer->getWidth() : 1280;
    int h = m_renderer ? m_renderer->getHeight() : 720;
    m_inputManager.processInput(w, h);
    playerInput = m_inputManager.getCarInput(w, h);

    if (m_mode == Mode::Race) {
        JPH::Vec3 carPositions[kTotalCars];
        JPH::Vec3 nextWPs[kTotalCars];
        for (int i = 0; i < kTotalCars; ++i) {
            if (m_cars[i]) {
                carPositions[i] = m_cars[i]->getPosition();
                int nextCp = m_lapManager.getNextCheckpoint(i);
                nextWPs[i] = m_track.getCheckpointPosition(nextCp);
            }
        }
        m_raceManager.update(dt, m_lapManager, carPositions, nextWPs, kTotalCars);

        if (m_raceManager.isFinished()) {
            m_finished = true;
        }
    }

    updateAI(dt);
    updateCars(playerInput, dt);
    updateCamera(dt);
}

void GameScene::updateAI(float dt) {
    if (m_mode == Mode::Practice) return;
    for (int i = 0; i < kAICount; ++i) {
        if (m_aiAgents[i] && m_cars[i + 1]) {
            CarInput aiInput = m_aiAgents[i]->update(dt);
            m_cars[i + 1]->update(aiInput, dt);
        }
    }
}

void GameScene::updateCars(const CarInput& playerInput, float dt) {
    if (m_cars[0]) {
        m_cars[0]->update(playerInput, dt);
    }
}

void GameScene::updateCamera(float dt) {
    if (m_cars[0]) {
        m_camera.updateChase(m_cars[0]->getPosition(), m_cars[0]->getForward(), dt);
    }
}

void GameScene::render() {
    if (!m_renderer || !m_uiRenderer) return;

    int w = m_renderer->getWidth();
    int h = m_renderer->getHeight();
    m_camera.setViewport(w, h);

    m_renderer->beginFrame();
    m_renderer->setCamera(m_camera);

    m_track.render(*m_renderer);
    for (int i = 0; i < (m_mode == Mode::Practice ? 1 : kTotalCars); ++i) {
        if (m_cars[i]) m_cars[i]->render(*m_renderer);
    }
    m_renderer->endFrame();

    m_renderer->beginUI();
    if (m_finished) {
        m_resultsScreen.render(m_uiRenderer, m_font, w, h, &m_lapManager, &m_raceManager);
    } else if (m_paused) {
        m_pauseMenu.render(m_uiRenderer, m_font, w, h);
    } else {
        m_hud.render(m_uiRenderer, m_font, w, h, m_cars[0].get(), &m_lapManager, &m_raceManager, m_mode == Mode::Practice);
    }
    m_renderer->endUI();
}

void GameScene::returnToMenu() {
    if (m_sceneManager) {
        m_sceneManager->changeScene(std::make_unique<MainMenuScene>(m_sceneManager, m_renderer, m_uiRenderer, m_font, m_audio, m_app));
    }
}
