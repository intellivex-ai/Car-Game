#include "scene/GameScene.h"
#include "scene/MainMenuScene.h"
#include "core/Log.h"

GameScene::GameScene(SceneManager* manager, IRenderer* renderer, UIRenderer* ui, Font* font, bool isPractice)
    : m_manager(manager), m_renderer(renderer), m_ui(ui), m_font(font), m_isPractice(isPractice) {
}

GameScene::~GameScene() = default;

void GameScene::onEnter() {
    LOGI("GameScene entered (Practice mode: %s)", m_isPractice ? "YES" : "NO");

    m_physics.init();
    m_track.buildOvalTrack(&m_physics);

    size_t numCars = m_isPractice ? 1 : 4;
    m_cars.resize(numCars);

    for (size_t i = 0; i < numCars; ++i) {
        m_cars[i] = std::make_unique<Car>();
        SpawnPoint sp = m_track.getSpawnPoint(i);
        std::string name = (i == 0) ? "Player" : "AI_" + std::to_string(i);
        m_cars[i]->init(&m_physics, sp.position, sp.rotation, name, i > 0);
    }

    // AI setup if in Race mode
    if (!m_isPractice) {
        m_aiAgents.resize(numCars - 1);
        const auto& waypoints = m_track.getAIWaypoints();
        for (size_t i = 1; i < numCars; ++i) {
            m_aiAgents[i - 1].init(m_cars[i].get(), waypoints, 0.8f + i * 0.05f);
        }
    }

    m_lapManager.init(numCars, m_track.getCheckpoints().size(), 3);
    m_physics.setCheckpointCallback([this](size_t carIndex, int cpIndex) {
        m_lapManager.onCheckpointPassed(carIndex, cpIndex);
    });

    std::vector<Car*> carPtrs;
    for (auto& car : m_cars) carPtrs.push_back(car.get());
    m_raceManager.init(&m_lapManager, carPtrs, &m_track);

    m_pauseMenu.setOnResume([this]() { m_isPaused = false; });
    m_pauseMenu.setOnMainMenu([this]() {
        m_manager->changeScene(std::make_unique<MainMenuScene>(m_manager, m_renderer, m_ui, m_font));
    });

    m_resultsScreen.setOnContinue([this]() {
        m_manager->changeScene(std::make_unique<MainMenuScene>(m_manager, m_renderer, m_ui, m_font));
    });

    if (!m_isPractice) {
        m_raceManager.startRace();
    }
}

void GameScene::onExit() {
    LOGI("GameScene exited");
    m_cars.clear();
    m_track.destroy();
    m_physics.shutdown();
}

void GameScene::update(float dt) {
    if (m_isPaused) return;

    // Fixed timestep physics update
    m_physics.step(dt);

    // Input processing
    int w = m_renderer ? m_renderer->getWidth() : 1280;
    int h = m_renderer ? m_renderer->getHeight() : 720;
    CarInput playerInput = m_inputManager.getCarInput(w, h);

    if (m_cars.size() > 0) {
        m_cars[0]->update(dt, playerInput);
    }

    // AI car updates
    for (size_t i = 0; i < m_aiAgents.size(); ++i) {
        CarInput aiInput = m_aiAgents[i].update(dt);
        if (i + 1 < m_cars.size()) {
            m_cars[i + 1]->update(dt, aiInput);
        }
    }

    // Camera follow player car
    if (m_cars.size() > 0) {
        m_camera.updateChase(m_cars[0]->getPosition(), m_cars[0]->getForwardVector(), dt);
    }

    if (!m_isPractice) {
        m_raceManager.update(dt);
        if (m_raceManager.getState() == RaceState::FINISHED) {
            m_isFinished = true;
        }
    }
}

void GameScene::render() {
    if (!m_renderer || !m_ui) return;

    int w = m_renderer->getWidth();
    int h = m_renderer->getHeight();
    m_camera.setViewport(w, h);

    JPH::Mat44 viewProj = m_camera.getViewProjectionMatrix();

    // 3D Scene Pass
    m_renderer->beginFrame();
    m_track.render(m_renderer, viewProj);
    for (auto& car : m_cars) {
        car->render(m_renderer, viewProj);
    }
    m_renderer->endFrame();

    // 2D HUD / Menu Overlay Pass
    m_ui->beginPass(w, h);

    if (m_isFinished) {
        m_resultsScreen.render(m_ui, m_font, w, h, &m_lapManager, &m_raceManager);
    } else if (m_isPaused) {
        m_pauseMenu.render(m_ui, m_font, w, h);
    } else {
        m_hud.render(m_ui, m_font, w, h, m_cars.size() > 0 ? m_cars[0].get() : nullptr,
                     &m_lapManager, &m_raceManager, m_isPractice);
    }

    m_ui->endPass();
}
