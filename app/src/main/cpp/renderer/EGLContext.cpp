#include "renderer/EGLContext.h"
#include "core/Log.h"

EGLContext_::~EGLContext_() {
    shutdown();
}

bool EGLContext_::init(ANativeWindow* window) {
    if (!window) return false;

    m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (m_display == EGL_NO_DISPLAY) {
        LOGE("EGLContext_: eglGetDisplay failed");
        return false;
    }

    if (!eglInitialize(m_display, nullptr, nullptr)) {
        LOGE("EGLContext_: eglInitialize failed");
        return false;
    }

    const EGLint attribs[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
        EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
        EGL_BLUE_SIZE,       8,
        EGL_GREEN_SIZE,      8,
        EGL_RED_SIZE,        8,
        EGL_ALPHA_SIZE,      8,
        EGL_DEPTH_SIZE,      16,
        EGL_NONE
    };

    EGLint numConfigs = 0;
    if (!eglChooseConfig(m_display, attribs, &m_config, 1, &numConfigs) || numConfigs <= 0) {
        LOGE("EGLContext_: eglChooseConfig failed");
        return false;
    }

    const EGLint contextAttribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_NONE
    };

    m_context = eglCreateContext(m_display, m_config, EGL_NO_CONTEXT, contextAttribs);
    if (m_context == EGL_NO_CONTEXT) {
        LOGE("EGLContext_: eglCreateContext failed");
        return false;
    }

    m_surface = eglCreateWindowSurface(m_display, m_config, window, nullptr);
    if (m_surface == EGL_NO_SURFACE) {
        LOGE("EGLContext_: eglCreateWindowSurface failed");
        return false;
    }

    if (!eglMakeCurrent(m_display, m_surface, m_surface, m_context)) {
        LOGE("EGLContext_: eglMakeCurrent failed");
        return false;
    }

    m_width = ANativeWindow_getWidth(window);
    m_height = ANativeWindow_getHeight(window);
    LOGI("EGLContext_: Initialized successfully (%dx%d)", m_width, m_height);
    return true;
}

void EGLContext_::destroySurface() {
    if (m_display != EGL_NO_DISPLAY && m_surface != EGL_NO_SURFACE) {
        eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroySurface(m_display, m_surface);
        m_surface = EGL_NO_SURFACE;
    }
}

bool EGLContext_::recreateSurface(ANativeWindow* window) {
    destroySurface();
    if (!window) return false;
    m_surface = eglCreateWindowSurface(m_display, m_config, window, nullptr);
    if (m_surface == EGL_NO_SURFACE) return false;
    return eglMakeCurrent(m_display, m_surface, m_surface, m_context);
}

void EGLContext_::onSurfaceChanged(int w, int h) {
    m_width = w;
    m_height = h;
}

bool EGLContext_::swapBuffers() {
    if (m_display != EGL_NO_DISPLAY && m_surface != EGL_NO_SURFACE) {
        return eglSwapBuffers(m_display, m_surface);
    }
    return false;
}

void EGLContext_::shutdown() {
    destroySurface();
    if (m_display != EGL_NO_DISPLAY) {
        if (m_context != EGL_NO_CONTEXT) {
            eglDestroyContext(m_display, m_context);
            m_context = EGL_NO_CONTEXT;
        }
        eglTerminate(m_display);
        m_display = EGL_NO_DISPLAY;
    }
}
