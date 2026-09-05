#pragma once
#include "gameplay/CarInput.h"
#include "gameplay/CarController.h"
#include "physics/VehiclePhysics.h"
#include "renderer/Mesh.h"
#include "renderer/Material.h"
#include "renderer/IRenderer.h"
#include <Jolt/Jolt.h>
#include <Jolt/Math/Vec3.h>
#include <cstdint>

/**
 * Car
 *
 * Top-level entity representing one car (player or AI) in the game world.
 *
 * Owns:
 *   - VehiclePhysics  (Jolt rigid body + VehicleConstraint)
 *   - CarController   (input → physics translation)
 *   - Visual mesh + material (placeholder box car)
 *
 * The Car does NOT own the CarInput — the caller provides it each frame,
 * either from InputManager (player) or AIAgent (AI opponent).
 *
 * Car IDs are sequential integers assigned by GameScene at spawn time.
 */
class Car {
public:
    explicit Car(int id);
    ~Car();

    Car(const Car&) = delete;
    Car& operator=(const Car&) = delete;

    /**
     * Creates the physics body and initializes visual mesh.
     * @param physicsSystem  Jolt physics world
     * @param spawnPos       Starting world position
     * @param mat            Visual material for this car
     */
    bool init(JPH::PhysicsSystem& physicsSystem,
              JPH::Vec3 spawnPos,
              Material mat);

    /**
     * Updates physics for this frame.
     * @param input  CarInput from either player or AI agent this frame
     * @param dt     Fixed timestep (seconds)
     */
    void update(const CarInput& input, float dt);

    /** Submits car geometry to the renderer. Call from render thread. */
    void render(IRenderer& renderer) const;

    /** Teleport car to position (for spawning / recovery). */
    void resetToPosition(JPH::PhysicsSystem& physicsSystem,
                         JPH::Vec3 pos, JPH::Quat rot);

    /** Remove from physics world. */
    void destroy(JPH::PhysicsSystem& physicsSystem);

    // Accessors
    int        getId()          const { return m_id; }
    JPH::Vec3  getPosition()    const { return m_vehicle.getPosition(); }
    JPH::Vec3  getForward()     const { return m_vehicle.getForwardVector(); }
    float      getSpeedKmh()    const { return m_vehicle.getSpeedKmh(); }
    JPH::BodyID getBodyID()     const { return m_vehicle.getBodyID(); }

    CarController&       getController()       { return m_controller; }
    const CarController& getController() const { return m_controller; }

private:
    int           m_id;
    VehiclePhysics m_vehicle;
    CarController  m_controller;

    Mesh     m_bodyMesh;
    Material m_material;
};
