#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Math/Vec3.h>

/**
 * Waypoint
 *
 * A single point in the AI navigation path.
 *
 * Fields:
 *   position     — world-space target position
 *   maxSpeedKmh  — the AI should not exceed this speed while approaching
 *                  this waypoint (lower for tight corners, higher for straights)
 *   radius       — distance at which this waypoint is considered "reached"
 */
struct Waypoint {
    JPH::Vec3 position   { 0.0f, 0.0f, 0.0f };
    float     maxSpeedKmh = 120.0f;            // km/h
    float     radius      = 8.0f;              // metres
};
