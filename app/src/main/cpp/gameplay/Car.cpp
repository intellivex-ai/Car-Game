#include "gameplay/Car.h"
#include "core/Log.h"

Car::Car() = default;
Car::~Car() {
    destroy();
}

void Car::init(PhysicsSystem* physics, JPH::Vec3Arg startPos, JPH::QuatArg startRot,
              const std::string& name, bool isAI) {
    m_name = name;
    m_isAI = isAI;
    m_vehicle.init(physics, startPos, startRot);
    m_controller.init(&m_vehicle);

    // Create visual mesh (box placeholder for car chassis)
    m_mesh = Mesh::createBox(JPH::Vec3(1.0f, 0.4f, 2.0f));
    m_material = Material::solidColor(isAI ? 0.8f : 0.2f, isAI ? 0.2f : 0.4f, isAI ? 0.2f : 0.9f);
}

void Car::update(float dt, const CarInput& input) {
    m_lastInput = input;
    m_controller.update(input, dt);
}

void Car::render(IRenderer* renderer, const JPH::Mat44& viewProj) {
    if (!renderer) return;
    JPH::Mat44 transform = m_vehicle.getTransform();
    m_material.bind(renderer);
    m_mesh.draw(viewProj * transform);
}

void Car::destroy() {
    m_mesh.destroy();
    m_vehicle.destroy();
}

JPH::Vec3 Car::getPosition() const {
    return m_vehicle.getPosition();
}

JPH::Quat Car::getRotation() const {
    return m_vehicle.getRotation();
}

JPH::Vec3 Car::getForwardVector() const {
    return m_vehicle.getForwardVector();
}

float Car::getSpeedKmh() const {
    return m_vehicle.getSpeedKmh();
}

void Car::resetToSpawn(JPH::Vec3Arg position, JPH::QuatArg rotation) {
    m_vehicle.setPositionAndRotation(position, rotation);
    m_vehicle.setVelocity(JPH::Vec3::sZero(), JPH::Vec3::sZero());
}
