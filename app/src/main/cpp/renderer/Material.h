#pragma once
#include "renderer/Texture.h"
#include <cstdint>

/**
 * Material
 *
 * Defines the visual properties of a rendered surface.
 * Supports a flat diffuse color (Phase 1–10) and an optional diffuse texture (Phase 11+).
 *
 * All values are plain data — no GL state inside this struct.
 * GLRenderer reads them when submitting a draw call.
 */
struct Material {
    // Diffuse base color (RGBA, each [0,1])
    float diffuseColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    // Optional texture. If nullptr or invalid, diffuseColor is used as a solid fill.
    const Texture* diffuseTexture = nullptr;

    // Simple specular highlight
    float specularStrength = 0.3f;
    float shininess        = 16.0f;

    // --- Convenience factories ---
    static Material solidColor(float r, float g, float b);
    static Material solidColor(float r, float g, float b, float a);
    static Material withTexture(const Texture* tex);

    // Preset placeholder materials
    static Material red()    { return solidColor(0.85f, 0.15f, 0.15f); }
    static Material blue()   { return solidColor(0.15f, 0.35f, 0.85f); }
    static Material green()  { return solidColor(0.15f, 0.70f, 0.25f); }
    static Material grey()   { return solidColor(0.45f, 0.45f, 0.45f); }
    static Material yellow() { return solidColor(0.95f, 0.85f, 0.10f); }
    static Material asphalt(){ return solidColor(0.25f, 0.25f, 0.28f); }
    static Material white()  { return solidColor(1.00f, 1.00f, 1.00f); }
};
