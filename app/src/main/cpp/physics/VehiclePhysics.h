#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Vehicle/VehicleConstraint.h>
#include <Jolt/Physics/Vehicle/WheeledVehicleController.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

/**
 * VehiclePhysics
 *
 * Wraps Jolt's VehicleConstraint to create an arcade-feel racing car.
 *
 * Architecture:
 *   - The car body is a BoxShape rigid body (approximating car dimensions)
 *   - A VehicleConstraint is attached to the body
 *   - The WheeledVehicleController drives engine torque, braking, and steering
 *   - Suspension and friction are tuned for arcade handling
 *
 * After create(), call apply() each fixed timestep with the current CarInput.
 * Query getPosition() / getForwardVector() for the camera and AI systems.
 *
 * All tuning constants are public and can be overridden before create() is called.
 */
class VehiclePhysics {
public:
    // --- Chassis dimensions ---
    float chassisLength   = 4.0f;    // metres
    float chassisWidth    = 1.8f;
    float chassisHeight   = 0.8f;
    float chassisMass     = 1200.0f; // kg

    // --- Suspension ---
    float suspRestLength  = 0.35f;   // metres
    float suspMaxLength   = 0.55f;
    float suspSpring      = 40000.0f; // N/m  (stiffness)
    float suspDamping     = 4000.0f;  // N·s/m

    // --- Drive ---
    float maxEngineTorque = 500.0f;   // N·m
    float maxBrakeTorque  = 1500.0f;  // N·m
    float maxSteerAngle   = 35.0f;    // degrees

    // --- Friction ---
    float wheelFrictionLong = 1.5f;   // longitudinal (forward grip)
    float wheelFrictionLat  = 2.0f;   // lateral (cornering grip)

    VehiclePhysics() = default;
    ~VehiclePhysics();

    VehiclePhysics(const VehiclePhysics&) = delete;
    VehiclePhysics& operator=(const VehiclePhysics&) = delete;

    /**
     * Creates the rigid body and attaches the VehicleConstraint to the physics world.
     * @param physicsSystem  The Jolt PhysicsSystem to add the body to
     * @param spawnPos       World-space spawn position (center of chassis)
     * @param objectLayer    Layers::VEHICLE
     */
    bool create(JPH::PhysicsSystem& physicsSystem,
                JPH::Vec3 spawnPos,
                JPH::ObjectLayer objectLayer);

    /**
     * Must be called each fixed timestep with the current control inputs.
     * Drives the VehicleConstraint controller.
     *
     * @param steering   [-1, 1]  left/right
     * @param throttle   [0, 1]
     * @param brake      [0, 1]
     * @param handbrake  true = rear wheels locked
     * @param dt         Fixed timestep (seconds)
     */
    void apply(float steering, float throttle, float brake, bool handbrake, float dt);

    /** Removes the vehicle from the physics world and frees the constraint. */
    void destroy(JPH::PhysicsSystem& physicsSystem);

    // --- State queries ---
    JPH::Vec3  getPosition()   const;
    JPH::Quat  getRotation()   const;
    JPH::Vec3  getLinearVelocity()  const;
    JPH::Vec3  getForwardVector()   const;
    float      getSpeedKmh()        const;

    JPH::BodyID getBodyID() const { return m_bodyID; }

    /** Teleports the car to a position (for spawning / reset). */
    void setPositionAndRotation(JPH::PhysicsSystem& physicsSystem,
                                JPH::Vec3 pos, JPH::Quat rot);

private:
    JPH::BodyID               m_bodyID;
    JPH::Ref<JPH::VehicleConstraint> m_constraint;
    JPH::PhysicsSystem*       m_physicsSystemPtr = nullptr;

    // Wheel body IDs for position queries (4 wheels: FL, FR, RL, RR)
    static constexpr int kWheelCount = 4;
};
