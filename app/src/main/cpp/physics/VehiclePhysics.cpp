// VehiclePhysics.cpp — Phase 6 implementation placeholder.
// Full VehicleConstraint setup implemented in Phase 6.
#include "physics/VehiclePhysics.h"
#include "core/Log.h"
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Vehicle/VehicleConstraint.h>
#include <Jolt/Physics/Vehicle/WheeledVehicleController.h>

VehiclePhysics::~VehiclePhysics() {
    // Destruction handled by destroy() — caller must call it before scene teardown.
}

bool VehiclePhysics::create(JPH::PhysicsSystem& physicsSystem,
                             JPH::Vec3 spawnPos,
                             JPH::ObjectLayer objectLayer) {
    LOGI("VehiclePhysics::create — stub (Phase 6)");
    m_physicsSystemPtr = &physicsSystem;
    (void)spawnPos; (void)objectLayer;
    // TODO Phase 6: create BoxShape, BodyCreationSettings, add body,
    //               configure VehicleConstraint with 4 wheels, suspension, engine.
    return false;
}

void VehiclePhysics::apply(float /*steering*/, float /*throttle*/,
                            float /*brake*/, bool /*handbrake*/, float /*dt*/) {
    // TODO Phase 6: call m_constraint->GetController<WheeledVehicleController>()
    //               and set inputs.
}

void VehiclePhysics::destroy(JPH::PhysicsSystem& physicsSystem) {
    if (!m_bodyID.IsInvalid()) {
        physicsSystem.GetBodyInterface().RemoveBody(m_bodyID);
        physicsSystem.GetBodyInterface().DestroyBody(m_bodyID);
        m_bodyID = JPH::BodyID();
    }
    m_constraint = nullptr;
    m_physicsSystemPtr = nullptr;
}

JPH::Vec3 VehiclePhysics::getPosition() const {
    if (!m_physicsSystemPtr || m_bodyID.IsInvalid()) return JPH::Vec3::sZero();
    return m_physicsSystemPtr->GetBodyInterface().GetPosition(m_bodyID);
}

JPH::Quat VehiclePhysics::getRotation() const {
    if (!m_physicsSystemPtr || m_bodyID.IsInvalid()) return JPH::Quat::sIdentity();
    return m_physicsSystemPtr->GetBodyInterface().GetRotation(m_bodyID);
}

JPH::Vec3 VehiclePhysics::getLinearVelocity() const {
    if (!m_physicsSystemPtr || m_bodyID.IsInvalid()) return JPH::Vec3::sZero();
    return m_physicsSystemPtr->GetBodyInterface().GetLinearVelocity(m_bodyID);
}

JPH::Vec3 VehiclePhysics::getForwardVector() const {
    return getRotation().RotateAxisZ(); // Z-forward convention
}

float VehiclePhysics::getSpeedKmh() const {
    return getLinearVelocity().Length() * 3.6f;
}

void VehiclePhysics::setPositionAndRotation(JPH::PhysicsSystem& physicsSystem,
                                             JPH::Vec3 pos, JPH::Quat rot) {
    if (m_bodyID.IsInvalid()) return;
    physicsSystem.GetBodyInterface().SetPositionAndRotation(
        m_bodyID, pos, rot, JPH::EActivation::Activate);
    physicsSystem.GetBodyInterface().SetLinearVelocity(m_bodyID, JPH::Vec3::sZero());
    physicsSystem.GetBodyInterface().SetAngularVelocity(m_bodyID, JPH::Vec3::sZero());
}
