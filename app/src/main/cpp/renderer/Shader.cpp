// Shader.cpp — Phase 3 implementation placeholder.
#include "renderer/Shader.h"
#include "core/Log.h"
#include <vector>

Shader::~Shader() {
    destroy();
}

bool Shader::build(const char* /*vertSrc*/, const char* /*fragSrc*/) {
    LOGI("Shader::build — stub (Phase 3)");
    return false; // TODO Phase 3
}

void Shader::use() const {
    if (m_program) glUseProgram(m_program);
}

void Shader::destroy() {
    if (m_program) {
        glDeleteProgram(m_program);
        m_program = 0;
    }
}

void Shader::setInt(const char* name, int value) const {
    glUniform1i(glGetUniformLocation(m_program, name), value);
}

void Shader::setFloat(const char* name, float value) const {
    glUniform1f(glGetUniformLocation(m_program, name), value);
}

void Shader::setVec3(const char* name, const float* v3) const {
    glUniform3fv(glGetUniformLocation(m_program, name), 1, v3);
}

void Shader::setVec4(const char* name, const float* v4) const {
    glUniform4fv(glGetUniformLocation(m_program, name), 1, v4);
}

void Shader::setMat4(const char* name, const float* m16) const {
    glUniformMatrix4fv(glGetUniformLocation(m_program, name), 1, GL_FALSE, m16);
}

// static
GLuint Shader::compileStage(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint ok = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        GLint len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> log(len + 1);
        glGetShaderInfoLog(shader, len, nullptr, log.data());
        LOGE("Shader compile error:\n%s", log.data());
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}
