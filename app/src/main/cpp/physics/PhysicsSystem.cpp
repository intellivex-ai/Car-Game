// PhysicsSystem.cpp — Phase 5 implementation placeholder.
// Jolt init, body interface, and contact listener implemented in Phase 5.
#include "physics/PhysicsSystem.h"
#include "core/Log.h"
#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Physics/PhysicsSettings.h>

// ============================================================
// Inner contact listener (fires sensor callbacks)
// ============================================================
class PhysicsSystem::ContactListenerImpl : public JPH::ContactListener {
public:
    PhysicsSystem* owner = nullptr;

    void OnContactAdded(const JPH::Body& body1, const JPH::Body& body2,
                        const JPH::ContactManifold& /*manifold*/,
                        JPH::ContactSettings& /*settings*/) override {
        // TODO Phase 5: identify sensor bodies and fire owner->m_sensorCallback
        (void)body1; (void)body2;
    }
};

// ============================================================
// BroadPhaseLayerInterfaceImpl
// ============================================================
BroadPhaseLayerInterfaceImpl::BroadPhaseLayerInterfaceImpl() {
    m_objectToBroadPhase[Layers::STATIC]  = BroadPhaseLayers::NON_MOVING;
    m_objectToBroadPhase[Layers::VEHICLE] = BroadPhaseLayers::MOVING;
    m_objectToBroadPhase[Layers::SENSOR]  = BroadPhaseLayers::NON_MOVING;
}

JPH::BroadPhaseLayer
BroadPhaseLayerInterfaceImpl::GetBroadPhaseLayer(JPH::ObjectLayer layer) const {
    return m_objectToBroadPhase[layer];
}

// ============================================================
// ObjectLayerPairFilterImpl
// ============================================================
bool ObjectLayerPairFilterImpl::ShouldCollide(JPH::ObjectLayer o1,
                                               JPH::ObjectLayer o2) const {
    switch (o1) {
        case Layers::STATIC:  return o2 == Layers::VEHICLE;
        case Layers::VEHICLE: return o2 == Layers::STATIC  ||
                                     o2 == Layers::VEHICLE ||
                                     o2 == Layers::SENSOR;
        case Layers::SENSOR:  return o2 == Layers::VEHICLE;
        default:              return false;
    }
}

// ============================================================
// ObjectVsBroadPhaseLayerFilterImpl
// ============================================================
bool ObjectVsBroadPhaseLayerFilterImpl::ShouldCollide(
    JPH::ObjectLayer objectLayer, JPH::BroadPhaseLayer bpLayer) const {
    switch (objectLayer) {
        case Layers::STATIC:  return bpLayer == BroadPhaseLayers::MOVING;
        case Layers::VEHICLE: return true;
        case Layers::SENSOR:  return bpLayer == BroadPhaseLayers::MOVING;
        default:              return false;
    }
}

// ============================================================
// PhysicsSystem
// ============================================================
PhysicsSystem::PhysicsSystem()
    : m_tempAllocator(10 * 1024 * 1024)   // 10 MB scratch
    , m_jobSystem(JPH::cMaxPhysicsJobs)
{}

PhysicsSystem::~PhysicsSystem() {
    delete m_contactListener;

    if (m_initialized) {
        JPH::UnregisterTypes();
        delete JPH::Factory::sInstance;
        JPH::Factory::sInstance = nullptr;
    }
}

bool PhysicsSystem::init() {
    // Jolt requires these to be called once per process.
    JPH::RegisterDefaultAllocator();
    JPH::Factory::sInstance = new JPH::Factory();
    JPH::RegisterTypes();

    m_physicsSystem.Init(
        kMaxBodies, kNumBodyMutexes,
        kMaxBodyPairs, kMaxContactPairs,
        m_broadPhaseLayerInterface,
        m_objVsBpFilter,
        m_objPairFilter
    );

    m_contactListener = new ContactListenerImpl();
    m_contactListener->owner = this;
    m_physicsSystem.SetContactListener(m_contactListener);

    m_initialized = true;
    LOGI("PhysicsSystem: Jolt initialized (max bodies: %u)", kMaxBodies);
    return true;
}

void PhysicsSystem::step(float fixedDt) {
    if (!m_initialized) return;
    // collisionSteps = 1 is sufficient for 60 Hz fixed timestep.
    m_physicsSystem.Update(fixedDt, 1, &m_tempAllocator, &m_jobSystem);
}

JPH::BodyInterface& PhysicsSystem::getBodyInterface() {
    return m_physicsSystem.GetBodyInterface();
}
