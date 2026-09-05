#include "renderer/Shader.h"
#include "core/Log.h"
#include <vector>

Shader::~Shader() {
    destroy();
}

bool Shader::build(const char* vertSrc, const char* fragSrc) {
    GLuint vert = compileStage(GL_VERTEX_SHADER, vertSrc);
    if (!vert) return false;

    GLuint frag = compileStage(GL_FRAGMENT_SHADER, fragSrc);
    if (!frag) {
        glDeleteShader(vert);
        return false;
    }

    m_program = glCreateProgram();
    glAttachShader(m_program, vert);
    glAttachShader(m_program, frag);
    glLinkProgram(m_program);

    GLint status = GL_FALSE;
    glGetProgramiv(m_program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint len = 0;
        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> log(len);
        glGetProgramInfoLog(m_program, len, nullptr, log.data());
        LOGE("Shader link error: %s", log.data());
        destroy();
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
    return m_program != 0;
}

GLuint Shader::compileStage(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint status = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> log(len);
        glGetShaderInfoLog(shader, len, nullptr, log.data());
        LOGE("Shader compile error: %s", log.data());
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

void Shader::use() const {
    if (m_program != 0) {
        glUseProgram(m_program);
    }
}

void Shader::destroy() {
    if (m_program != 0) {
        glDeleteProgram(m_program);
        m_program = 0;
    }
}

void Shader::setInt(const char* name, int value) const {
    GLint loc = glGetUniformLocation(m_program, name);
    if (loc >= 0) glUniform1i(loc, value);
}

void Shader::setFloat(const char* name, float value) const {
    GLint loc = glGetUniformLocation(m_program, name);
    if (loc >= 0) glUniform1f(loc, value);
}

void Shader::setVec3(const char* name, const float* v3) const {
    GLint loc = glGetUniformLocation(m_program, name);
    if (loc >= 0) glUniform3fv(loc, 1, v3);
}

void Shader::setVec4(const char* name, const float* v4) const {
    GLint loc = glGetUniformLocation(m_program, name);
    if (loc >= 0) glUniform4fv(loc, 1, v4);
}

void Shader::setMat4(const char* name, const float* m16) const {
    GLint loc = glGetUniformLocation(m_program, name);
    if (loc >= 0) glUniformMatrix4fv(loc, 1, GL_FALSE, m16);
}
