#include "scene/SceneManager.h"
#include "core/Log.h"

SceneManager::SceneManager() = default;

void SceneManager::pushScene(std::unique_ptr<IScene> scene) {
    if (!scene) return;
    if (!m_scenes.empty()) {
        m_scenes.back()->onExit();
    }
    m_scenes.push_back(std::move(scene));
    m_scenes.back()->onEnter();
}

void SceneManager::popScene() {
    if (m_scenes.empty()) return;
    m_scenes.back()->onExit();
    m_scenes.pop_back();
    if (!m_scenes.empty()) {
        m_scenes.back()->onEnter();
    }
}

void SceneManager::changeScene(std::unique_ptr<IScene> scene) {
    while (!m_scenes.empty()) {
        m_scenes.back()->onExit();
        m_scenes.pop_back();
    }
    pushScene(std::move(scene));
}

void SceneManager::update(float dt) {
    if (!m_scenes.empty()) {
        m_scenes.back()->update(dt);
    }
}

void SceneManager::render() {
    if (!m_scenes.empty()) {
        m_scenes.back()->render();
    }
}
