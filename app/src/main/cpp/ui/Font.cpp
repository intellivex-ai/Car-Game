#include "ui/Font.h"
#include "core/Log.h"
#include <vector>
#include <cstring>

Font::~Font() {
    destroy();
}

bool Font::init() {
    std::vector<uint8_t> pixels(kAtlasWidth * kAtlasHeight * 4, 255);

    glGenTextures(1, &m_atlasTexture);
    glBindTexture(GL_TEXTURE_2D, m_atlasTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, kAtlasWidth, kAtlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    LOGI("Font: Procedural atlas initialized");
    return true;
}

void Font::drawText(UIRenderer& ui, const char* text, float x, float y, float scale,
                   float r, float g, float b, float a) const {
    if (!text) return;

    float curX = x;
    float glyphW = static_cast<float>(kGlyphWidth) * scale;
    float glyphH = static_cast<float>(kGlyphHeight) * scale;

    size_t len = std::strlen(text);
    for (size_t i = 0; i < len; ++i) {
        char c = text[i];
        if (c == ' ') {
            curX += glyphW * 0.6f;
            continue;
        }

        int index = static_cast<int>(c) - 32;
        if (index < 0 || index >= kGlyphCols * kGlyphRows) index = 0;

        int col = index % kGlyphCols;
        int row = index / kGlyphCols;

        float u0 = static_cast<float>(col * kGlyphWidth) / static_cast<float>(kAtlasWidth);
        float v0 = static_cast<float>(row * kGlyphHeight) / static_cast<float>(kAtlasHeight);
        float u1 = u0 + static_cast<float>(kGlyphWidth) / static_cast<float>(kAtlasWidth);
        float v1 = v0 + static_cast<float>(kGlyphHeight) / static_cast<float>(kAtlasHeight);

        float uv[4] = { u0, v0, u1, v1 };
        ui.drawTexturedRect(curX, y, glyphW, glyphH, uv, r, g, b, a, m_atlasTexture);
        curX += glyphW * 0.7f;
    }
}

float Font::measureText(const char* text, float scale) const {
    if (!text) return 0.0f;
    size_t len = std::strlen(text);
    return static_cast<float>(len) * static_cast<float>(kGlyphWidth) * scale * 0.7f;
}

void Font::destroy() {
    if (m_atlasTexture != 0) {
        glDeleteTextures(1, &m_atlasTexture);
        m_atlasTexture = 0;
    }
}
