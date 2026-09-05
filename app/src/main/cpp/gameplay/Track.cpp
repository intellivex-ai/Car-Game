#include "gameplay/Track.h"
#include "core/Log.h"

Track::Track() = default;
Track::~Track() {
    destroy();
}

void Track::buildOvalTrack(PhysicsSystem* physics) {
    m_physics = physics;

    // Build visual ground plane mesh (200x200)
    m_groundMesh = Mesh::createPlane(200.0f, 200.0f);
    m_groundMaterial = Material::solidColor(0.2f, 0.25f, 0.2f); // Dark asphalt track tone

    // Build static ground body in physics
    if (physics) {
        physics->createStaticGround(200.0f, 200.0f);
    }

    // Set spawn points on grid
    m_spawnPoints[0] = { JPH::Vec3(  0.0f, 0.5f,  60.0f), JPH::Quat::sIdentity() };
    m_spawnPoints[1] = { JPH::Vec3( -4.0f, 0.5f,  65.0f), JPH::Quat::sIdentity() };
    m_spawnPoints[2] = { JPH::Vec3(  4.0f, 0.5f,  65.0f), JPH::Quat::sIdentity() };
    m_spawnPoints[3] = { JPH::Vec3(  0.0f, 0.5f,  70.0f), JPH::Quat::sIdentity() };

    // Build checkpoints around simple oval layout
    m_checkpoints.clear();
    // Finish / Checkpoint 0
    m_checkpoints.emplace_back(0, JPH::Vec3(0.0f, 1.5f, 50.0f), JPH::Vec3(15.0f, 3.0f, 2.0f));
    // Turn 1
    m_checkpoints.emplace_back(1, JPH::Vec3(40.0f, 1.5f, 25.0f), JPH::Vec3(15.0f, 3.0f, 2.0f));
    // Back straight
    m_checkpoints.emplace_back(2, JPH::Vec3(40.0f, 1.5f, -25.0f), JPH::Vec3(15.0f, 3.0f, 2.0f));
    // Turn 2
    m_checkpoints.emplace_back(3, JPH::Vec3(0.0f, 1.5f, -50.0f), JPH::Vec3(15.0f, 3.0f, 2.0f));
    // Turn 3
    m_checkpoints.emplace_back(4, JPH::Vec3(-40.0f, 1.5f, -25.0f), JPH::Vec3(15.0f, 3.0f, 2.0f));
    // Turn 4
    m_checkpoints.emplace_back(5, JPH::Vec3(-40.0f, 1.5f, 25.0f), JPH::Vec3(15.0f, 3.0f, 2.0f));

    if (physics) {
        for (auto& cp : m_checkpoints) {
            cp.createSensorBody(physics);
        }
    }

    // AI waypoints following oval track curvature
    m_aiWaypoints.clear();
    m_aiWaypoints.push_back({ JPH::Vec3(  0.0f, 0.0f,  50.0f), 120.0f, 5.0f });
    m_aiWaypoints.push_back({ JPH::Vec3( 35.0f, 0.0f,  35.0f),  80.0f, 5.0f });
    m_aiWaypoints.push_back({ JPH::Vec3( 45.0f, 0.0f,   0.0f), 100.0f, 5.0f });
    m_aiWaypoints.push_back({ JPH::Vec3( 35.0f, 0.0f, -35.0f),  80.0f, 5.0f });
    m_aiWaypoints.push_back({ JPH::Vec3(  0.0f, 0.0f, -50.0f), 120.0f, 5.0f });
    m_aiWaypoints.push_back({ JPH::Vec3(-35.0f, 0.0f, -35.0f),  80.0f, 5.0f });
    m_aiWaypoints.push_back({ JPH::Vec3(-45.0f, 0.0f,   0.0f), 100.0f, 5.0f });
    m_aiWaypoints.push_back({ JPH::Vec3(-35.0f, 0.0f,  35.0f),  80.0f, 5.0f });

    LOGI("Track: Oval track generated with %zu checkpoints and %zu AI waypoints",
         m_checkpoints.size(), m_aiWaypoints.size());
}

void Track::render(IRenderer* renderer, const JPH::Mat44& viewProj) {
    if (!renderer) return;
    m_groundMaterial.bind(renderer);
    m_groundMesh.draw(viewProj);
}

void Track::destroy() {
    if (m_physics) {
        for (auto& cp : m_checkpoints) {
            cp.destroy(m_physics);
        }
    }
    m_groundMesh.destroy();
}

SpawnPoint Track::getSpawnPoint(size_t index) const {
    if (index < m_spawnPoints.size()) return m_spawnPoints[index];
    return { JPH::Vec3::sZero(), JPH::Quat::sIdentity() };
}

Checkpoint* Track::getCheckpoint(size_t index) {
    if (index < m_checkpoints.size()) return &m_checkpoints[index];
    return nullptr;
}
