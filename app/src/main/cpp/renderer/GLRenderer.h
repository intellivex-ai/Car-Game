#pragma once
#include "renderer/IRenderer.h"
#include "renderer/EGLContext.h"
#include "renderer/Shader.h"
#include <GLES3/gl3.h>
#include <memory>

class GLRenderer : public IRenderer {
public:
    GLRenderer() = default;
    ~GLRenderer() override;

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
    EGLContext_   m_egl;
    Shader        m_sceneShader;
    Shader        m_uiShader;

    int           m_width  = 0;
    int           m_height = 0;
    bool          m_uiPass = false;

    bool initShaders();
    void buildOrthoMatrix(float l, float r, float b, float t);
};
