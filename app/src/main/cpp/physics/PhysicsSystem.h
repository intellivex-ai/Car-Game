#pragma once
#include "physics/CollisionLayers.h"
#include <Jolt/Jolt.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemSingleThreaded.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/ContactListener.h>
#include <functional>
#include <cstdint>

/**
 * PhysicsSystem
 *
 * Initializes and manages the Jolt physics world for the game.
 *
 * Wraps:
 *   - JPH::PhysicsSystem (the world)
 *   - TempAllocatorImpl (scratch memory for Jolt internals)
 *   - JobSystemSingleThreaded (keeps physics on the game thread for MVP;
 *     can be upgraded to multi-threaded later)
 *   - Custom contact listener for checkpoint sensor triggers
 *
 * One PhysicsSystem instance lives per GameScene.
 * It is destroyed with the scene (race end) and re-created for the next race.
 *
 * Tuning constants:
 *   kMaxBodies       — max simultaneous physics bodies
 *   kMaxContactPairs — max simultaneous contact pairs per step
 *   kNumBodyMutexes  — internal locking granularity
 */
class PhysicsSystem {
public:
    // Sensor trigger callback: (carBodyId, checkpointIndex)
    using SensorCallback = std::function<void(JPH::BodyID carId, int checkpointIndex)>;

    static constexpr uint32_t kMaxBodies       = 256;
    static constexpr uint32_t kMaxBodyPairs    = 1024;
    static constexpr uint32_t kMaxContactPairs = 1024;
    static constexpr uint32_t kNumBodyMutexes  = 0;  // 0 = auto

    PhysicsSystem();
    ~PhysicsSystem();

    PhysicsSystem(const PhysicsSystem&) = delete;
    PhysicsSystem& operator=(const PhysicsSystem&) = delete;

    /**
     * Initializes Jolt and creates the physics world.
     * Must be called before any bodies are created.
     */
    bool init();

    /**
     * Advances the physics simulation by fixedDt seconds.
     * Call once per fixed timestep from GameLoop::Update.
     */
    void step(float fixedDt);

    /**
     * Registers a callback fired when a vehicle body enters a sensor checkpoint.
     * The callback receives the vehicle's BodyID and the checkpoint's user data index.
     */
    void setSensorCallback(SensorCallback cb) { m_sensorCallback = std::move(cb); }

    JPH::PhysicsSystem& getJoltSystem() { return m_physicsSystem; }
    JPH::BodyInterface& getBodyInterface();

private:
    bool                          m_initialized = false;
    JPH::TempAllocatorImpl        m_tempAllocator;
    JPH::JobSystemSingleThreaded  m_jobSystem;
    BroadPhaseLayerInterfaceImpl  m_broadPhaseLayerInterface;
    ObjectVsBroadPhaseLayerFilterImpl m_objVsBpFilter;
    ObjectLayerPairFilterImpl     m_objPairFilter;
    JPH::PhysicsSystem            m_physicsSystem;
    SensorCallback                m_sensorCallback;

    // Inner contact listener class
    class ContactListenerImpl;
    ContactListenerImpl*          m_contactListener = nullptr;
};
