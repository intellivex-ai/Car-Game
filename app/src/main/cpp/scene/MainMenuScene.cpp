#include "scene/MainMenuScene.h"
#include "scene/GameScene.h"
#include "core/Log.h"

MainMenuScene::MainMenuScene(SceneManager* sceneManager, IRenderer* renderer,
                             UIRenderer* ui, Font* font, AudioManager* audio, android_app* app)
    : m_sceneManager(sceneManager), m_renderer(renderer), m_ui(ui), m_font(font), m_audio(audio), m_app(app) {
    m_menu.setOnPractice([this]() { navigateToPractice(); });
    m_menu.setOnRace([this]() { navigateToRace(); });
}

void MainMenuScene::onEnter() {
    LOGI("MainMenuScene entered");
}

void MainMenuScene::onExit() {
    LOGI("MainMenuScene exited");
}

void MainMenuScene::update(float dt) {}

void MainMenuScene::render() {
    if (!m_ui || !m_renderer) return;
    int w = m_renderer->getWidth();
    int h = m_renderer->getHeight();

    m_renderer->beginUI();
    m_menu.render(m_ui, m_font, w, h);
    m_renderer->endUI();
}

void MainMenuScene::navigateToPractice() {
    if (m_sceneManager) {
        m_sceneManager->changeScene(std::make_unique<GameScene>(GameScene::Mode::Practice, m_sceneManager, m_renderer, m_ui, m_font, m_audio, m_app));
    }
}

void MainMenuScene::navigateToRace() {
    if (m_sceneManager) {
        m_sceneManager->changeScene(std::make_unique<GameScene>(GameScene::Mode::Race, m_sceneManager, m_renderer, m_ui, m_font, m_audio, m_app));
    }
}
