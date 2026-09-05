#pragma once

/**
 * CarInput
 *
 * Plain-data struct representing the desired control state of one car.
 *
 * This is the single boundary between the input system (or AI agent) and
 * the CarController. Neither the InputManager nor the AIAgent should ever
 * touch VehiclePhysics directly.
 *
 * All values are normalized:
 *   steering:  -1.0 = full left,  +1.0 = full right
 *   throttle:   0.0 = off,         1.0 = full throttle
 *   brake:      0.0 = released,    1.0 = full braking
 *   handbrake:  false / true
 */
struct CarInput {
    float steering  = 0.0f;
    float throttle  = 0.0f;
    float brake     = 0.0f;
    bool  handbrake = false;

    /** Returns a zero-input state (coasting with no steering). */
    static CarInput neutral() { return {}; }
};
