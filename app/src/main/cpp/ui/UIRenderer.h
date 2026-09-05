#pragma once
#include <GLES3/gl3.h>
#include "renderer/Shader.h"

/**
 * UIRenderer
 *
 * 2D orthographic quad renderer for all HUD and menu elements.
 *
 * Used exclusively inside IRenderer's beginUI()/endUI() pass.
 * Coordinates are in screen pixels (0,0 = top-left, screenW×screenH = bottom-right).
 *
 * Capabilities:
 *   - drawRect: filled colored rectangle with optional alpha
 *   - drawRectBorder: unfilled border rectangle
 *   - drawTexturedRect: rectangle sampled from a texture atlas (used for font)
 *
 * Batches up to kMaxQuadsPerFlush quads before flushing to the GPU.
 * Call flush() at the end of each UI pass, or it's called automatically when full.
 */
class UIRenderer {
public:
    static constexpr int kMaxQuadsPerFlush = 512;

    UIRenderer() = default;
    ~UIRenderer();

    /**
     * Creates the UI shader and uploads vertex buffer objects.
     * Must be called after the GL context is ready.
     */
    bool init(int screenWidth, int screenHeight);

    void destroy();
    void onScreenChanged(int w, int h);

    /** Flush all pending quads to the GPU. Call at end of UI pass. */
    void flush();

    /**
     * Draw a solid-colored filled rectangle.
     * x, y = top-left pixel coords; w, h = size in pixels; rgba ∈ [0,1]
     */
    void drawRect(float x, float y, float w, float h,
                  float r, float g, float b, float a = 1.0f);

    /**
     * Draw a rectangle from a texture atlas (used by Font).
     * srcUV = {u0,v0,u1,v1} in normalized texture coordinates.
     */
    void drawTexturedRect(float x, float y, float w, float h,
                          float srcUV[4],
                          float r, float g, float b, float a,
                          GLuint textureId);

private:
    struct QuadVertex {
        float x, y;       // screen coords
        float u, v;       // texcoords
        float r, g, b, a; // color
    };

    Shader  m_shader;
    GLuint  m_vao = 0;
    GLuint  m_vbo = 0;

    QuadVertex  m_verts[kMaxQuadsPerFlush * 4] = {};
    int         m_quadCount = 0;
    GLuint      m_currentTexture = 0;

    int m_screenW = 1;
    int m_screenH = 1;

    float m_ortho[16] = {};

    void buildOrtho();
    void flushInternal();
};
