#include "renderer/GLRenderer.h"
#include "renderer/Mesh.h"
#include "renderer/Material.h"
#include "renderer/Camera.h"
#include "core/Log.h"

GLRenderer::~GLRenderer() {
    onSurfaceDestroyed();
}

bool GLRenderer::onSurfaceCreated(ANativeWindow* window) {
    if (!m_egl.init(window)) {
        LOGE("GLRenderer: EGL context initialization failed");
        return false;
    }
    LOGI("GLRenderer: EGL context created");
    return initShaders();
}

void GLRenderer::onSurfaceChanged(int width, int height) {
    m_width = width;
    m_height = height;
    glViewport(0, 0, width, height);
    m_egl.onSurfaceChanged(width, height);
}

void GLRenderer::onSurfaceDestroyed() {
    m_sceneShader.destroy();
    m_uiShader.destroy();
    m_egl.shutdown();
}

void GLRenderer::beginFrame() {
    glClearColor(0.1f, 0.12f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

void GLRenderer::endFrame() {
    m_egl.swapBuffers();
}

void GLRenderer::drawMesh(const Mesh& mesh, const Material& material, const JPH::Mat44& worldTransform) {
    m_sceneShader.bind();
    mesh.draw(worldTransform);
}

void GLRenderer::setCamera(const Camera& camera) {
    // Camera transform caching
}

void GLRenderer::beginUI() {
    m_uiPass = true;
    glDisable(GL_DEPTH_TEST);
}

void GLRenderer::endUI() {
    m_uiPass = false;
    glEnable(GL_DEPTH_TEST);
}

bool GLRenderer::initShaders() {
    return true;
}

void GLRenderer::buildOrthoMatrix(float l, float r, float b, float t) {}
