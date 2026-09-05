#pragma once
#include "ui/UIRenderer.h"
#include <GLES3/gl3.h>
#include <cstdint>

/**
 * Font
 *
 * Bitmap font renderer using an embedded ASCII atlas.
 *
 * The atlas is a 256×128 pixel texture with a 16×8 grid of characters.
 * Each glyph cell is 16×16 pixels.
 * Printable ASCII (0x20–0x7E) is supported.
 *
 * The raw pixel data is embedded as a C array in Font.cpp (generated from
 * a 1-bit monochrome bitmap). This avoids any asset file dependency for text.
 *
 * Usage:
 *   font.drawText(uiRenderer, "LAP 1/3", x, y, scale, r, g, b, a);
 */
class Font {
public:
    static constexpr int kAtlasWidth   = 256;
    static constexpr int kAtlasHeight  = 128;
    static constexpr int kGlyphCols    = 16;
    static constexpr int kGlyphRows    = 8;
    static constexpr int kGlyphWidth   = kAtlasWidth  / kGlyphCols;  // 16 px
    static constexpr int kGlyphHeight  = kAtlasHeight / kGlyphRows;  // 16 px

    Font() = default;
    ~Font();

    /**
     * Uploads the embedded font atlas to the GPU.
     * Must be called after GL context is ready.
     */
    bool init();
    void destroy();

    /**
     * Draws a null-terminated ASCII string.
     * @param x, y    Top-left pixel position
     * @param scale   Multiplier on glyph size (1.0 = 16px glyphs)
     * @param r,g,b,a Tint color ∈ [0,1]
     */
    void drawText(UIRenderer& ui,
                  const char* text,
                  float x, float y,
                  float scale,
                  float r, float g, float b, float a = 1.0f) const;

    /** Returns width in pixels of a string at given scale. */
    float measureText(const char* text, float scale) const;

private:
    GLuint m_atlasTexture = 0;
};
