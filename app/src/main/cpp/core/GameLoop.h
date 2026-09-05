#pragma once
#include <functional>
#include <cstdint>

/**
 * GameLoop
 *
 * Fixed-timestep update loop with uncapped rendering.
 *
 * Physics and gameplay run at a fixed rate (kPhysicsHz = 60 Hz by default).
 * Rendering runs as fast as the display allows, with an interpolation alpha
 * passed to the render callback to smooth positions between physics steps.
 *
 * The "spiral of death" is prevented by capping the max number of physics
 * steps per frame to kMaxStepsPerFrame (default 4).
 */
class GameLoop {
public:
    using UpdateFn = std::function<void(float dt)>;
    using RenderFn = std::function<void(float alpha)>;

    static constexpr float    kPhysicsHz        = 60.0f;
    static constexpr float    kFixedDt          = 1.0f / kPhysicsHz;
    static constexpr int      kMaxStepsPerFrame  = 4;

    GameLoop() = default;

    /**
     * Registers callbacks. Call before tick().
     *   updateFn(dt)    — called at fixed kFixedDt intervals
     *   renderFn(alpha) — called once per frame; alpha ∈ [0,1] for interpolation
     */
    void setCallbacks(UpdateFn updateFn, RenderFn renderFn);

    /**
     * Advances the loop by realDeltaTime seconds (wall-clock time since last tick).
     * Call once per frame from Application::run().
     */
    void tick(double realDeltaTime);

    /** Resets the accumulator (e.g., after a long pause to avoid catch-up). */
    void reset();

private:
    UpdateFn m_updateFn;
    RenderFn m_renderFn;
    double   m_accumulator = 0.0;
};
