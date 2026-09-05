#include "gameplay/Car.h"
#include "core/Log.h"

Car::Car(int id) : m_id(id) {}

Car::~Car() = default;

bool Car::init(JPH::PhysicsSystem& physicsSystem, JPH::Vec3 spawnPos, Material mat) {
    m_material = mat;
    if (!m_vehicle.init(physicsSystem, spawnPos, JPH::Quat::sIdentity())) {
        LOGE("Car %d: Failed to initialize vehicle physics", m_id);
        return false;
    }
    m_controller.init(&m_vehicle);
    m_bodyMesh = Mesh::createBox(2.0f, 0.8f, 4.0f);
    return true;
}

void Car::update(const CarInput& input, float dt) {
    m_controller.update(input, dt);
}

void Car::render(IRenderer& renderer) const {
    renderer.drawMesh(m_bodyMesh, m_material, m_vehicle.getTransform());
}

void Car::resetToPosition(JPH::PhysicsSystem& physicsSystem, JPH::Vec3 pos, JPH::Quat rot) {
    m_vehicle.setPositionAndRotation(pos, rot);
    m_vehicle.setVelocity(JPH::Vec3::sZero(), JPH::Vec3::sZero());
}

void Car::destroy(JPH::PhysicsSystem& physicsSystem) {
    m_bodyMesh.destroy();
    m_vehicle.destroy(physicsSystem);
}
