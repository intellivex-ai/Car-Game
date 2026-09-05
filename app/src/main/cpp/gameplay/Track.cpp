#include "gameplay/Track.h"
#include "core/Log.h"

bool Track::create(JPH::PhysicsSystem& physicsSystem) {
    m_groundMesh = Mesh::createPlane(200.0f, 200.0f);
    m_asphaltMat = Material::solidColor(0.2f, 0.25f, 0.2f);

    m_spawnPoints = {
        { JPH::Vec3( 0.0f, 0.5f, 60.0f), JPH::Quat::sIdentity() },
        { JPH::Vec3(-4.0f, 0.5f, 65.0f), JPH::Quat::sIdentity() },
        { JPH::Vec3( 4.0f, 0.5f, 65.0f), JPH::Quat::sIdentity() },
        { JPH::Vec3( 0.0f, 0.5f, 70.0f), JPH::Quat::sIdentity() }
    };

    m_waypoints = {
        JPH::Vec3(  0.0f, 0.0f,  50.0f),
        JPH::Vec3( 35.0f, 0.0f,  35.0f),
        JPH::Vec3( 45.0f, 0.0f,   0.0f),
        JPH::Vec3( 35.0f, 0.0f, -35.0f),
        JPH::Vec3(  0.0f, 0.0f, -50.0f),
        JPH::Vec3(-35.0f, 0.0f, -35.0f),
        JPH::Vec3(-45.0f, 0.0f,   0.0f),
        JPH::Vec3(-35.0f, 0.0f,  35.0f)
    };

    m_checkpoints.emplace_back(0, JPH::Vec3(0.0f, 1.5f, 50.0f), JPH::Vec3(15.0f, 3.0f, 2.0f));
    m_checkpoints.emplace_back(1, JPH::Vec3(40.0f, 1.5f, 0.0f), JPH::Vec3(15.0f, 3.0f, 2.0f));
    m_checkpoints.emplace_back(2, JPH::Vec3(0.0f, 1.5f, -50.0f), JPH::Vec3(15.0f, 3.0f, 2.0f));
    m_checkpoints.emplace_back(3, JPH::Vec3(-40.0f, 1.5f, 0.0f), JPH::Vec3(15.0f, 3.0f, 2.0f));

    for (auto& cp : m_checkpoints) {
        cp.createSensorBody(&physicsSystem);
    }

    LOGI("Track: Oval track created with %zu checkpoints", m_checkpoints.size());
    return true;
}

void Track::destroy(JPH::PhysicsSystem& physicsSystem) {
    for (auto& cp : m_checkpoints) {
        cp.destroy(&physicsSystem);
    }
    m_groundMesh.destroy();
}

void Track::render(IRenderer& renderer) const {
    renderer.drawMesh(m_groundMesh, m_asphaltMat, JPH::Mat44::sIdentity());
}

JPH::Vec3 Track::getSpawnPoint(int carIndex) const {
    if (carIndex >= 0 && carIndex < static_cast<int>(m_spawnPoints.size())) {
        return m_spawnPoints[carIndex].position;
    }
    return JPH::Vec3::sZero();
}

JPH::Vec3 Track::getCheckpointPosition(int index) const {
    if (index >= 0 && index < static_cast<int>(m_checkpoints.size())) {
        return m_checkpoints[index].getPosition();
    }
    return JPH::Vec3::sZero();
}
