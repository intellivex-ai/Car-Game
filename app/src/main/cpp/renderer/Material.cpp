#include "renderer/Material.h"

Material Material::solidColor(float r, float g, float b) {
    return solidColor(r, g, b, 1.0f);
}

Material Material::solidColor(float r, float g, float b, float a) {
    Material m;
    m.diffuseColor[0] = r;
    m.diffuseColor[1] = g;
    m.diffuseColor[2] = b;
    m.diffuseColor[3] = a;
    return m;
}

Material Material::withTexture(const Texture* tex) {
    Material m;
    m.diffuseTexture = tex;
    return m;
}
