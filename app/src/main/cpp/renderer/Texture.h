#pragma once
#include <GLES3/gl3.h>
#include <cstdint>

/**
 * Texture
 *
 * Wraps a GL_TEXTURE_2D object.
 *
 * Phase 1–10: flat-color 1×1 textures created via createSolidColor()
 * Phase 11+:  PNG loading via stb_image from AAssetManager
 */
class Texture {
public:
    Texture() = default;
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&&) noexcept;
    Texture& operator=(Texture&&) noexcept;

    /**
     * Creates a 1×1 RGBA texture of the given color.
     * r, g, b, a each in [0, 255].
     * Used for placeholder colored geometry.
     */
    static Texture createSolidColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

    /**
     * Creates a texture from raw RGBA8 pixels.
     * @param pixels  Pointer to width*height*4 bytes of RGBA data
     */
    bool uploadRGBA(const uint8_t* pixels, int width, int height);

    /** Binds to texture unit (default: unit 0). */
    void bind(int unit = 0) const;
    void unbind() const;
    void destroy();

    bool isValid() const { return m_id != 0; }
    GLuint id()    const { return m_id; }

private:
    GLuint m_id = 0;
};
