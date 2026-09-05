#pragma once
#include <EGL/egl.h>
#include <android/native_window.h>

class EGLContext_ {
public:
    EGLContext_() = default;
    ~EGLContext_();

    bool init(ANativeWindow* window);
    void destroySurface();
    bool recreateSurface(ANativeWindow* window);
    void onSurfaceChanged(int w, int h);
    void shutdown();
    void destroy() { shutdown(); }

    bool swapBuffers();
    bool isReady() const { return m_surface != EGL_NO_SURFACE; }

    int getWidth()  const { return m_width; }
    int getHeight() const { return m_height; }

private:
    EGLDisplay m_display = EGL_NO_DISPLAY;
    EGLContext m_context = EGL_NO_CONTEXT;
    EGLSurface m_surface = EGL_NO_SURFACE;
    EGLConfig  m_config  = nullptr;

    int m_width  = 0;
    int m_height = 0;
};
