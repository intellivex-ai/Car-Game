#include "gameplay/Checkpoint.h"
#include "core/Log.h"

Checkpoint::Checkpoint(int index, JPH::Vec3Arg position, JPH::Vec3Arg extents)
    : m_index(index), m_position(position), m_extents(extents) {}

void Checkpoint::createSensorBody(PhysicsSystem* physics) {
    if (!physics) return;
    m_bodyID = physics->createSensorBox(m_position, m_extents, m_index);
}

void Checkpoint::destroy(PhysicsSystem* physics) {
    if (physics && !m_bodyID.IsInvalid()) {
        physics->destroyBody(m_bodyID);
        m_bodyID = JPH::BodyID();
    }
}
