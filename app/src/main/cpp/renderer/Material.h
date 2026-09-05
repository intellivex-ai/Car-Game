#pragma once
#include "renderer/Texture.h"
#include <cstdint>

class IRenderer;

struct Material {
    float diffuseColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    const Texture* diffuseTexture = nullptr;
    float specularStrength = 0.3f;
    float shininess        = 16.0f;

    void bind(const IRenderer* renderer = nullptr) const { (void)renderer; }

    static Material solidColor(float r, float g, float b);
    static Material solidColor(float r, float g, float b, float a);
    static Material withTexture(const Texture* tex);

    static Material red()    { return solidColor(0.85f, 0.15f, 0.15f); }
    static Material blue()   { return solidColor(0.15f, 0.35f, 0.85f); }
    static Material green()  { return solidColor(0.15f, 0.70f, 0.25f); }
    static Material grey()   { return solidColor(0.45f, 0.45f, 0.45f); }
    static Material yellow() { return solidColor(0.95f, 0.85f, 0.10f); }
    static Material asphalt(){ return solidColor(0.25f, 0.25f, 0.28f); }
    static Material white()  { return solidColor(1.00f, 1.00f, 1.00f); }
};
