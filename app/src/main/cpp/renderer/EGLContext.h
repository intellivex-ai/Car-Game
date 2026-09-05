#pragma once
#include <EGL/egl.h>
#include <android/native_window.h>

/**
 * EGLContext_
 *
 * Manages the EGL display, context, and surface lifecycle.
 * Trailing underscore avoids clash with the EGLContext typedef from <EGL/egl.h>.
 *
 * Creates an OpenGL ES 3.0 context with:
 *   - 24-bit color depth
 *   - 16-bit depth buffer
 *   - No MSAA initially (can be enabled later)
 */
class EGLContext_ {
public:
    EGLContext_() = default;
    ~EGLContext_();

    /**
     * Creates display, config, context, and window surface.
     * Returns false on failure; caller should abort.
     */
    bool init(ANativeWindow* window);

    /**
     * Destroys the window surface (e.g., when Activity is paused).
     * The context and display are kept alive for fast resume.
     */
    void destroySurface();

    /**
     * Re-creates the window surface after destroySurface().
     * Call when the window is available again.
     */
    bool recreateSurface(ANativeWindow* window);

    /** Destroys everything — context, surface, display. */
    void destroy();

    /** Presents the back buffer. Returns false if the surface is lost. */
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

    bool chooseConfig();
    bool createContext();
};
