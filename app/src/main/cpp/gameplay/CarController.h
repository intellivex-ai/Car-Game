#pragma once
#include "gameplay/CarInput.h"
#include "physics/VehiclePhysics.h"

/**
 * CarController
 *
 * Translates a CarInput into calls on VehiclePhysics each fixed timestep.
 *
 * This is the only class that knows both CarInput and VehiclePhysics —
 * all other code is isolated to one side of this boundary.
 *
 * Additional controller-level behaviors (separate from raw physics):
 *   - Input smoothing (optional lerp on steering to reduce jitter)
 *   - Speed limiter (for AI fairness)
 *   - Reverse detection (detect when car is stopped and reverse is requested)
 */
class CarController {
public:
    // Input smoothing: steering input is lerped toward target each step.
    // 1.0 = instant (no smoothing), lower = more lag
    float steeringSmoothingAlpha = 0.25f;

    // Speed limit in km/h (0 = no limit). Used to nerf AI slightly.
    float maxSpeedKmh = 0.0f;

    CarController() = default;

    /**
     * Connects the controller to its physics body.
     * Must be called once after VehiclePhysics::create().
     */
    void init(VehiclePhysics* vehicle);

    /**
     * Apply one fixed-timestep's worth of input.
     * Call from Car::update() after LapManager and AI/Input have produced a CarInput.
     */
    void update(const CarInput& input, float dt);

    /** Reset internal state (e.g., after car is teleported to a spawn point). */
    void reset();

    VehiclePhysics* getVehicle() { return m_vehicle; }

private:
    VehiclePhysics* m_vehicle = nullptr;
    float           m_currentSteering = 0.0f;
};
