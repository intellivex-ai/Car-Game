// Texture.cpp — Phase 3 implementation placeholder.
#include "renderer/Texture.h"
#include "core/Log.h"

Texture::~Texture() { destroy(); }

Texture::Texture(Texture&& other) noexcept : m_id(other.m_id) {
    other.m_id = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) { destroy(); m_id = other.m_id; other.m_id = 0; }
    return *this;
}

// static
Texture Texture::createSolidColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    Texture t;
    uint8_t pixel[4] = { r, g, b, a };
    t.uploadRGBA(pixel, 1, 1);
    return t;
}

bool Texture::uploadRGBA(const uint8_t* pixels, int width, int height) {
    if (m_id) glDeleteTextures(1, &m_id);
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    return (glGetError() == GL_NO_ERROR);
}

void Texture::bind(int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::destroy() {
    if (m_id) { glDeleteTextures(1, &m_id); m_id = 0; }
}
