#pragma once
#include <GLES3/gl3.h>
#include <string>

class Shader {
public:
    Shader() = default;
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    bool build(const char* vertSrc, const char* fragSrc);

    void use() const;
    void bind() const { use(); }
    void destroy();

    bool isValid() const { return m_program != 0; }
    GLuint program() const { return m_program; }

    void setInt  (const char* name, int value)              const;
    void setBool (const char* name, bool value)             const { setInt(name, value ? 1 : 0); }
    void setFloat(const char* name, float value)            const;
    void setVec3 (const char* name, const float* v3)        const;
    void setVec4 (const char* name, const float* v4)        const;
    void setMat4 (const char* name, const float* m16)       const;

private:
    GLuint m_program = 0;

    static GLuint compileStage(GLenum type, const char* src);
};
