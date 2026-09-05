#pragma once
#include <GLES3/gl3.h>
#include <string>

/**
 * Shader
 *
 * Compiles and links a GLSL vertex + fragment shader pair.
 * Provides uniform upload helpers.
 *
 * Usage:
 *   Shader s;
 *   s.build(vertSrc, fragSrc);
 *   s.use();
 *   s.setMat4("u_MVP", matrix);
 */
class Shader {
public:
    Shader() = default;
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    /**
     * Compiles vertex and fragment GLSL source, links into a program.
     * Returns false and logs the error on failure.
     */
    bool build(const char* vertSrc, const char* fragSrc);

    void use() const;
    void destroy();

    bool isValid() const { return m_program != 0; }
    GLuint program() const { return m_program; }

    // Uniform helpers — must call use() before these
    void setInt  (const char* name, int value)              const;
    void setFloat(const char* name, float value)            const;
    void setVec3 (const char* name, const float* v3)        const;
    void setVec4 (const char* name, const float* v4)        const;
    void setMat4 (const char* name, const float* m16)       const;

private:
    GLuint m_program = 0;

    static GLuint compileStage(GLenum type, const char* src);
};
