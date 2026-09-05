#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Math/Vec3.h>

/**
 * Checkpoint
 *
 * An axis-aligned box trigger volume placed at a specific point on the track.
 * Cars passing through checkpoints in sequence are used to validate laps.
 *
 * In the Jolt world, each checkpoint is a sensor body (isSensor = true)
 * with Layers::SENSOR as its object layer. The PhysicsSystem's contact listener
 * fires the sensor callback when a vehicle body overlaps it.
 *
 * The sequenceIndex must be passed in order (0, 1, 2, ..., N) for a valid lap.
 */
class Checkpoint {
public:
    Checkpoint() = default;

    /**
     * Creates the sensor body in the physics world.
     * @param physicsSystem  Jolt physics world
     * @param center         World center of the trigger volume
     * @param halfExtents    Half-size of the AABB trigger (e.g., {10, 2, 2})
     * @param sequenceIndex  Order index (0-based). Stored in Jolt body user data.
     */
    bool create(JPH::PhysicsSystem& physicsSystem,
                JPH::Vec3 center,
                JPH::Vec3 halfExtents,
                int sequenceIndex);

    void destroy(JPH::PhysicsSystem& physicsSystem);

    int         getIndex()  const { return m_index; }
    JPH::BodyID getBodyID() const { return m_bodyID; }
    JPH::Vec3   getCenter() const { return m_center; }

private:
    JPH::BodyID m_bodyID;
    JPH::Vec3   m_center { 0, 0, 0 };
    int         m_index  = -1;
};
