#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Math/Vec3.h>
#include <Jolt/Math/Mat44.h>

/**
 * Camera
 *
 * Computes view and projection matrices for the 3D scene.
 *
 * Two modes:
 *   - Free camera (position + target, mostly for debugging)
 *   - Chase camera (follows a car with smooth lag)
 *
 * The chase camera trails the car at a configurable distance and height,
 * lerping both position and look-at target each frame to reduce jitter.
 *
 * Matrices are stored as column-major float[16] arrays compatible with
 * GL's glUniformMatrix4fv (GL_FALSE transpose).
 */
class Camera {
public:
    // Projection parameters
    float fovDegrees = 65.0f;
    float nearPlane  = 0.5f;
    float farPlane   = 500.0f;
    int   viewWidth  = 1;
    int   viewHeight = 1;

    // Chase camera offsets (in car's local space before world rotation is applied)
    float chaseDistance = 8.0f;   // metres behind the car
    float chaseHeight   = 3.0f;   // metres above the car
    float lerpPosition  = 0.08f;  // lerp alpha for camera position (per fixed step)
    float lerpLookAt    = 0.12f;  // lerp alpha for look-at target

    /**
     * Sets a free camera facing a given target.
     */
    void lookAt(JPH::Vec3 eye, JPH::Vec3 target, JPH::Vec3 up = JPH::Vec3(0,1,0));

    /**
     * Chase camera update. Call once per fixed timestep.
     * @param carPosition   World position of the car's center of mass
     * @param carForward    Normalized forward direction of the car
     * @param dt            Fixed timestep (seconds)
     */
    void updateChase(JPH::Vec3 carPosition, JPH::Vec3 carForward, float dt);

    /** Recomputes and returns the view matrix (column-major float[16]). */
    const float* getViewMatrix() const  { return m_view; }

    /** Recomputes and returns the projection matrix (column-major float[16]). */
    const float* getProjMatrix() const  { return m_proj; }

    /** Call when screen dimensions change. */
    void setViewport(int width, int height);

private:
    float m_view[16] = {};
    float m_proj[16] = {};

    JPH::Vec3 m_eye    { 0, 5, -10 };
    JPH::Vec3 m_target { 0, 0,   0  };

    void buildView();
    void buildProj();
    void lookAtInternal(JPH::Vec3 eye, JPH::Vec3 target, JPH::Vec3 up);
};
