// GLRenderer.cpp — Phase 3 implementation placeholder.
#include "renderer/GLRenderer.h"
#include "core/Log.h"

GLRenderer::~GLRenderer() {
    onSurfaceDestroyed();
}

bool GLRenderer::onSurfaceCreated(ANativeWindow* window) {
    LOGI("GLRenderer::onSurfaceCreated — stub (Phase 3)");
    return m_egl.init(window);
}

void GLRenderer::onSurfaceChanged(int width, int height) {
    m_width  = width;
    m_height = height;
    LOGI("GLRenderer::onSurfaceChanged: %d x %d", width, height);
}

void GLRenderer::onSurfaceDestroyed() {
    m_egl.destroy();
}

void GLRenderer::beginFrame() {
    // TODO Phase 3: glClear, set viewport
}

void GLRenderer::endFrame() {
    m_egl.swapBuffers();
}

void GLRenderer::drawMesh(const Mesh& /*mesh*/,
                           const Material& /*material*/,
                           const JPH::Mat44& /*worldTransform*/) {
    // TODO Phase 3
}

void GLRenderer::setCamera(const Camera& /*camera*/) {
    // TODO Phase 3
}

void GLRenderer::beginUI() {
    m_uiPass = true;
    // TODO Phase 3: disable depth test, set ortho
}

void GLRenderer::endUI() {
    m_uiPass = false;
    // TODO Phase 3: re-enable depth test
}

bool GLRenderer::initShaders() {
    return false; // TODO Phase 3
}

void GLRenderer::buildOrthoMatrix(float /*l*/, float /*r*/, float /*b*/, float /*t*/) {
    // TODO Phase 3
}
