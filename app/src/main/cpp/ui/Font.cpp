#include "ui/Font.h"
#include "core/Log.h"
#include <vector>

Font::Font() = default;
Font::~Font() {
    destroy();
}

bool Font::initProceduralAtlas() {
    // Generate a basic 128x128 procedural 8x16 font atlas
    const int atlasWidth = 128;
    const int atlasHeight = 128;
    std::vector<uint8_t> pixels(atlasWidth * atlasHeight * 4, 255); // White square placeholder for font

    glGenTextures(1, &m_atlasTexture);
    glBindTexture(GL_TEXTURE_2D, m_atlasTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    LOGI("Font: Procedural atlas initialized");
    return true;
}

void Font::drawText(UIRenderer* ui, const std::string& text, float x, float y, float scale, const float color[4]) {
    if (!ui) return;

    float curX = x;
    const float charW = 12.0f * scale;
    const float charH = 20.0f * scale;

    for (char c : text) {
        if (c == ' ') {
            curX += charW;
            continue;
        }

        // Draw character quad using atlas
        ui->drawTexturedQuad(curX, y, charW, charH, m_atlasTexture, 0.0f, 0.0f, 1.0f, 1.0f, color);
        curX += charW;
    }
}

void Font::destroy() {
    if (m_atlasTexture) {
        glDeleteTextures(1, &m_atlasTexture);
        m_atlasTexture = 0;
    }
}
