#include "scene/MainMenuScene.h"
#include "scene/GameScene.h"
#include "core/Log.h"

MainMenuScene::MainMenuScene(SceneManager* manager, IRenderer* renderer, UIRenderer* ui, Font* font)
    : m_manager(manager), m_renderer(renderer), m_ui(ui), m_font(font) {
    m_mainMenu.setOnPractice([this]() {
        LOGI("MainMenuScene: Practice selected");
        if (m_manager) {
            m_manager->changeScene(std::make_unique<GameScene>(m_manager, m_renderer, m_ui, m_font, true));
        }
    });

    m_mainMenu.setOnRace([this]() {
        LOGI("MainMenuScene: Race selected");
        if (m_manager) {
            m_manager->changeScene(std::make_unique<GameScene>(m_manager, m_renderer, m_ui, m_font, false));
        }
    });
}

void MainMenuScene::onEnter() {
    LOGI("MainMenuScene entered");
}

void MainMenuScene::onExit() {
    LOGI("MainMenuScene exited");
}

void MainMenuScene::update(float dt) {
    // Menu animation/update logic
}

void MainMenuScene::render() {
    if (!m_ui || !m_renderer) return;
    int w = m_renderer->getWidth();
    int h = m_renderer->getHeight();

    m_ui->beginPass(w, h);
    m_mainMenu.render(m_ui, m_font, w, h);
    m_ui->endPass();
}
