#pragma once
#include "gameplay/Checkpoint.h"
#include "physics/PhysicsSystem.h"
#include "renderer/Mesh.h"
#include "renderer/Material.h"
#include "renderer/IRenderer.h"
#include <Jolt/Jolt.h>
#include <Jolt/Math/Vec3.h>
#include <Jolt/Math/Quat.h>
#include <vector>

struct SpawnPoint {
    JPH::Vec3 position;
    JPH::Quat rotation;
};

class Track {
public:
    Track() = default;
    ~Track() = default;

    Track(const Track&) = delete;
    Track& operator=(const Track&) = delete;

    bool create(JPH::PhysicsSystem& physicsSystem);
    bool buildOvalTrack(JPH::PhysicsSystem& physicsSystem) { return create(physicsSystem); }
    void destroy(JPH::PhysicsSystem& physicsSystem);
    void render(IRenderer& renderer) const;

    int getCheckpointCount() const { return (int)m_checkpoints.size(); }
    const std::vector<Checkpoint>& getCheckpoints() const { return m_checkpoints; }
    JPH::Vec3 getCheckpointPosition(int index) const;

    JPH::Vec3 getSpawnPoint(int carIndex) const;
    const std::vector<JPH::Vec3>& getWaypoints() const { return m_waypoints; }
    const std::vector<JPH::Vec3>& getAIWaypoints() const { return m_waypoints; }

    float getTrackLength() const { return m_trackLength; }
    const char* getName() const { return "Oval Circuit"; }

private:
    std::vector<Checkpoint> m_checkpoints;
    std::vector<SpawnPoint> m_spawnPoints;
    std::vector<JPH::Vec3>  m_waypoints;
    float                   m_trackLength = 0.0f;

    Mesh     m_groundMesh;
    Mesh     m_wallMesh;
    Mesh     m_startLineMesh;
    Material m_asphaltMat;
    Material m_wallMat;
    Material m_startLineMat;

    std::vector<JPH::BodyID> m_staticBodyIDs;

    void buildGeometry();
    void buildPhysics(JPH::PhysicsSystem& physicsSystem);
    void buildCheckpoints(JPH::PhysicsSystem& physicsSystem);
    void buildWaypoints();
};
