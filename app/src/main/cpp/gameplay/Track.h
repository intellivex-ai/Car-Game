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
#include <array>

/**
 * SpawnPoint
 *
 * Position and rotation for one car at race start.
 */
struct SpawnPoint {
    JPH::Vec3 position;
    JPH::Quat rotation;
};

/**
 * Track
 *
 * Represents the racing environment for one race session.
 *
 * MVP implementation: hardcoded oval circuit.
 * Future: load from asset file.
 *
 * Responsibilities:
 *   - Render track surface (asphalt mesh)
 *   - Create Jolt static bodies for:
 *       - Ground plane
 *       - Outer and inner walls
 *   - Own and manage checkpoint sensor bodies
 *   - Provide spawn points
 *   - Provide AI waypoints
 */
class Track {
public:
    Track() = default;
    ~Track() = default;

    Track(const Track&) = delete;
    Track& operator=(const Track&) = delete;

    /**
     * Builds the track in the given physics world.
     * Creates ground, walls, and checkpoint sensors.
     */
    bool create(JPH::PhysicsSystem& physicsSystem);

    /** Removes all track bodies from the physics world. */
    void destroy(JPH::PhysicsSystem& physicsSystem);

    /** Renders track geometry. */
    void render(IRenderer& renderer) const;

    // --- Accessors ---
    int getCheckpointCount() const { return (int)m_checkpoints.size(); }
    const std::vector<Checkpoint>& getCheckpoints() const { return m_checkpoints; }

    /** Spawn point for car at index (0 = player, 1–3 = AI). */
    SpawnPoint getSpawnPoint(int carIndex) const;

    /** AI waypoints in track order. */
    const std::vector<JPH::Vec3>& getWaypoints() const { return m_waypoints; }

    /** Approximate track length in metres. */
    float getTrackLength() const { return m_trackLength; }

    /** Name displayed in UI. */
    const char* getName() const { return "Oval Circuit"; }

private:
    std::vector<Checkpoint> m_checkpoints;
    std::vector<SpawnPoint> m_spawnPoints;
    std::vector<JPH::Vec3>  m_waypoints;
    float                   m_trackLength = 0.0f;

    // Visual geometry
    Mesh     m_groundMesh;
    Mesh     m_wallMesh;
    Mesh     m_startLineMesh;
    Material m_asphaltMat;
    Material m_wallMat;
    Material m_startLineMat;

    // Physics bodies
    std::vector<JPH::BodyID> m_staticBodyIDs;

    void buildGeometry();
    void buildPhysics(JPH::PhysicsSystem& physicsSystem);
    void buildCheckpoints(JPH::PhysicsSystem& physicsSystem);
    void buildWaypoints();
};
