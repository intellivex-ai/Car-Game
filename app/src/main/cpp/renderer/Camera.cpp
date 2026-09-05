// Camera.cpp — Phase 8 implementation placeholder.
// Chase camera and matrix math implemented in Phase 8.
#include "renderer/Camera.h"
#include <cmath>
#include <cstring>

void Camera::setViewport(int width, int height) {
    viewWidth  = width;
    viewHeight = height;
    buildProj();
}

void Camera::lookAt(JPH::Vec3 eye, JPH::Vec3 target, JPH::Vec3 up) {
    m_eye    = eye;
    m_target = target;
    lookAtInternal(eye, target, up);
    buildProj();
}

void Camera::updateChase(JPH::Vec3 carPosition, JPH::Vec3 carForward, float /*dt*/) {
    // TODO Phase 8: smooth chase camera with lerp
    JPH::Vec3 desiredPos = carPosition - carForward * chaseDistance
                           + JPH::Vec3(0, chaseHeight, 0);
    m_eye    = desiredPos;
    m_target = carPosition;
    lookAtInternal(m_eye, m_target, JPH::Vec3(0,1,0));
    buildProj();
}

void Camera::lookAtInternal(JPH::Vec3 eye, JPH::Vec3 target, JPH::Vec3 up) {
    JPH::Vec3 f = (target - eye).Normalized();
    JPH::Vec3 r = f.Cross(up).Normalized();
    JPH::Vec3 u = r.Cross(f);

    // Column-major view matrix (GL convention)
    m_view[0]  =  r.GetX(); m_view[4]  =  r.GetY(); m_view[8]  =  r.GetZ(); m_view[12] = -r.Dot(eye);
    m_view[1]  =  u.GetX(); m_view[5]  =  u.GetY(); m_view[9]  =  u.GetZ(); m_view[13] = -u.Dot(eye);
    m_view[2]  = -f.GetX(); m_view[6]  = -f.GetY(); m_view[10] = -f.GetZ(); m_view[14] =  f.Dot(eye);
    m_view[3]  =  0.0f;     m_view[7]  =  0.0f;     m_view[11] =  0.0f;     m_view[15] =  1.0f;
}

void Camera::buildProj() {
    float aspect = (viewHeight > 0)
        ? static_cast<float>(viewWidth) / static_cast<float>(viewHeight)
        : 1.0f;
    float fovRad = fovDegrees * (3.14159265358979323846f / 180.0f);
    float f = 1.0f / std::tan(fovRad * 0.5f);

    std::memset(m_proj, 0, sizeof(m_proj));
    m_proj[0]  = f / aspect;
    m_proj[5]  = f;
    m_proj[10] = (farPlane + nearPlane) / (nearPlane - farPlane);
    m_proj[11] = -1.0f;
    m_proj[14] = (2.0f * farPlane * nearPlane) / (nearPlane - farPlane);
}
