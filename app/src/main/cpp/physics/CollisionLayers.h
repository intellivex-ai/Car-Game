#pragma once

// ---------------------------------------------------------------------------
// CollisionLayers
//
// Defines Jolt Physics object layers for broadphase filtering.
// Keep the number of layers small — each layer pair adds broadphase overhead.
// ---------------------------------------------------------------------------

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

namespace Layers {
    // Object layers (used to filter which bodies collide with which)
    static constexpr JPH::ObjectLayer STATIC   = 0;  // Track ground, walls (never moves)
    static constexpr JPH::ObjectLayer VEHICLE  = 1;  // Cars (player + AI)
    static constexpr JPH::ObjectLayer SENSOR   = 2;  // Checkpoint trigger volumes (isSensor)
    static constexpr JPH::ObjectLayer NUM_LAYERS = 3;
}

namespace BroadPhaseLayers {
    // Broadphase layers — coarser groupings for the broadphase tree
    static constexpr JPH::BroadPhaseLayer NON_MOVING { 0 };
    static constexpr JPH::BroadPhaseLayer MOVING     { 1 };
    static constexpr uint32_t NUM_LAYERS = 2;
}

/**
 * ObjectLayerPairFilter
 *
 * Determines which object layer pairs can collide.
 *
 * Collision table:
 *   STATIC  ↔ VEHICLE  = YES  (cars hit the track)
 *   VEHICLE ↔ VEHICLE  = YES  (cars can collide with each other)
 *   SENSOR  ↔ VEHICLE  = YES  (checkpoints detect cars)
 *   STATIC  ↔ STATIC   = NO   (static bodies never interact)
 *   STATIC  ↔ SENSOR   = NO
 *   SENSOR  ↔ SENSOR   = NO
 */
class ObjectLayerPairFilterImpl final : public JPH::ObjectLayerPairFilter {
public:
    bool ShouldCollide(JPH::ObjectLayer obj1, JPH::ObjectLayer obj2) const override;
};

/**
 * BroadPhaseLayerInterface
 *
 * Maps object layers to broadphase layers.
 */
class BroadPhaseLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface {
public:
    BroadPhaseLayerInterfaceImpl();

    uint32_t GetNumBroadPhaseLayers() const override {
        return BroadPhaseLayers::NUM_LAYERS;
    }
    JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer layer) const override;

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer layer) const override;
#endif

private:
    JPH::BroadPhaseLayer m_objectToBroadPhase[Layers::NUM_LAYERS];
};

/**
 * ObjectVsBroadPhaseLayerFilter
 */
class ObjectVsBroadPhaseLayerFilterImpl final : public JPH::ObjectVsBroadPhaseLayerFilter {
public:
    bool ShouldCollide(JPH::ObjectLayer layer1,
                       JPH::BroadPhaseLayer layer2) const override;
};
