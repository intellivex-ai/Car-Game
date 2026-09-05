#pragma once
#include <android/native_window.h>
#include <Jolt/Jolt.h>
#include <Jolt/Math/Mat44.h>

class Mesh;
struct Material;
class Camera;

class IRenderer {
public:
    virtual ~IRenderer() = default;

    virtual bool onSurfaceCreated(ANativeWindow* window) = 0;
    virtual void onSurfaceChanged(int width, int height) = 0;
    virtual void onSurfaceDestroyed() = 0;

    virtual void beginFrame() = 0;
    virtual void endFrame()   = 0;

    virtual void drawMesh(const Mesh& mesh,
                          const Material& material,
                          const JPH::Mat44& worldTransform) = 0;

    virtual void setCamera(const Camera& camera) = 0;

    virtual void beginUI()  = 0;
    virtual void endUI()    = 0;

    virtual int getWidth()  const = 0;
    virtual int getHeight() const = 0;
};
