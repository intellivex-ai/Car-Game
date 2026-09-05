#pragma once
#include <memory>
#include <functional>

/**
 * IScene
 *
 * Abstract base class for all game scenes.
 *
 * Lifecycle:
 *   onEnter()  — called when scene becomes active
 *   update(dt) — called each fixed timestep
 *   render()   — called each frame (after update)
 *   onExit()   — called before scene is replaced
 */
class IScene {
public:
    virtual ~IScene() = default;

    virtual void onEnter() {}
    virtual void onExit()  {}

    virtual void update(float dt) = 0;
    virtual void render()         = 0;

    /** Return true if this scene should block input from scenes below it on the stack. */
    virtual bool blocksInput() const { return true; }
};
