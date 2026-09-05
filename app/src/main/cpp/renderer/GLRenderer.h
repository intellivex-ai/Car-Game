#pragma once
#include "renderer/IRenderer.h"
#include "renderer/EGLContext.h"
#include "renderer/Shader.h"
#include <GLES3/gl3.h>
#include <memory>

/**
 * GLRenderer
 *
 * OpenGL ES 3.0 implementation of IRenderer.
 *
 * Owns the EGL context, the main 3D shader program, and the UI shader program.
 * All GL state management is centralized here — gameplay code never calls GL directly.
 *
 * Rendering architecture:
 *   beginFrame()   → glClear, set viewport
 *   setCamera()    → upload view/proj matrices to UBO
 *   drawMesh()     → bind VAO, upload model matrix, glDrawElements
 *   beginUI()      → switch to ortho projection, disable depth test
 *   endUI()
 *   endFrame()     → eglSwapBuffers
 */
class GLRenderer : public IRenderer {
public:
    GLRenderer() = default;
    ~GLRenderer() override;

    // IRenderer interface
    bool onSurfaceCreated(ANativeWindow* window) override;
    void onSurfaceChanged(int width, int height) override;
    void onSurfaceDestroyed() override;

    void beginFrame() override;
    void endFrame()   override;

    void drawMesh(const Mesh& mesh,
                  const Material& material,
                  const JPH::Mat44& worldTransform) override;

    void setCamera(const Camera& camera) override;

    void beginUI() override;
    void endUI()   override;

    int getWidth()  const override { return m_width; }
    int getHeight() const override { return m_height; }

private:
    EGLContext_   m_egl;          // underscore suffix avoids clash with EGL's EGLContext typedef
    Shader        m_sceneShader;
    Shader        m_uiShader;

    // Cached per-frame matrices (uploaded to shader at setCamera)
    float         m_viewMatrix[16]  = {};
    float         m_projMatrix[16]  = {};
    float         m_orthoMatrix[16] = {};

    int           m_width  = 0;
    int           m_height = 0;
    bool          m_uiPass = false;

    bool initShaders();
    void buildOrthoMatrix(float l, float r, float b, float t);
};
