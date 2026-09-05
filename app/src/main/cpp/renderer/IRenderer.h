#pragma once

/**
 * IRenderer
 *
 * Abstract renderer interface.
 *
 * All gameplay and scene code accesses rendering exclusively through this
 * interface, keeping the implementation (OpenGL ES 3.0 today, Vulkan later)
 * entirely behind a stable API.
 *
 * To add a Vulkan backend:
 *   1. Create VkRenderer : public IRenderer
 *   2. Implement all pure virtual methods
 *   3. Select at runtime in Application based on device capability
 *
 * Render state resets to a known-clean state at the start of each beginFrame().
 * Callers should not assume state carries across frames.
 */

// Forward declarations — avoids pulling GL/Vulkan headers into gameplay code
struct Mesh;
struct Material;
class  Camera;

// Jolt math types used for world transforms
#include <Jolt/Jolt.h>
#include <Jolt/Math/Mat44.h>

class IRenderer {
public:
    virtual ~IRenderer() = default;

    /**
     * Called when the native window surface is created or resized.
     * The renderer should (re-)initialize its swap chain / framebuffers here.
     */
    virtual bool onSurfaceCreated(ANativeWindow* window) = 0;
    virtual void onSurfaceChanged(int width, int height) = 0;
    virtual void onSurfaceDestroyed() = 0;

    /** Frame boundary markers. */
    virtual void beginFrame() = 0;
    virtual void endFrame()   = 0;

    /** Submit a mesh for rendering with a given material and world transform. */
    virtual void drawMesh(const Mesh& mesh,
                          const Material& material,
                          const JPH::Mat44& worldTransform) = 0;

    /** Set the camera for this frame. Must be called between beginFrame/endFrame. */
    virtual void setCamera(const Camera& camera) = 0;

    /** 2D UI overlay pass (orthographic, depth-test off). */
    virtual void beginUI()  = 0;
    virtual void endUI()    = 0;

    /** Screen dimensions (updated in onSurfaceChanged). */
    virtual int getWidth()  const = 0;
    virtual int getHeight() const = 0;
};
