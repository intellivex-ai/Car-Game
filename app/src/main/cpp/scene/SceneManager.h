#pragma once
#include "scene/IScene.h"
#include <memory>
#include <vector>
#include <functional>

/**
 * SceneManager
 *
 * Manages a stack of IScene instances.
 *
 * Operations are deferred: requests made during update/render are applied
 * at the start of the next frame to avoid iterator invalidation.
 *
 * Usage:
 *   - replace(scene)  — pop all scenes, push new one (common: menu → game)
 *   - push(scene)     — push on top (common: game → pause overlay)
 *   - pop()           — remove top (common: dismiss pause menu)
 *
 * Only the top scene receives update() and render() calls.
 */
class SceneManager {
public:
    SceneManager() = default;

    /** Replace entire stack with a new scene. */
    void replace(std::unique_ptr<IScene> scene);

    /** Push a scene on top. */
    void push(std::unique_ptr<IScene> scene);

    /** Pop the top scene. */
    void pop();

    /**
     * Apply pending scene changes, then update and render the top scene.
     * Call once per game loop iteration.
     */
    void update(float dt);
    void render();

    bool isEmpty() const { return m_stack.empty(); }
    IScene* top()  const { return m_stack.empty() ? nullptr : m_stack.back().get(); }

private:
    std::vector<std::unique_ptr<IScene>> m_stack;

    enum class PendingOp { Replace, Push, Pop };
    struct Pending {
        PendingOp              op;
        std::unique_ptr<IScene> scene;
    };
    std::vector<Pending> m_pendingOps;

    void applyPending();
};
