#include "ui/UIRenderer.h"
#include "core/Log.h"

UIRenderer::UIRenderer() = default;
UIRenderer::~UIRenderer() {
    destroy();
}

bool UIRenderer::init() {
    // Phase 9 UI quad shader initialization
    const char* vertShaderSrc = R"(#version 300 es
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

uniform vec2 uScreenSize;

out vec2 vTexCoord;

void main() {
    // Map screen pixel coords (0..width, 0..height) to normalized device coords (-1..1, 1..-1)
    vec2 ndc = (aPos / uScreenSize) * 2.0 - 1.0;
    ndc.y = -ndc.y; // invert Y for top-left origin
    gl_Position = vec4(ndc, 0.0, 1.0);
    vTexCoord = aTexCoord;
}
)";

    const char* fragShaderSrc = R"(#version 300 es
precision mediump float;

in vec2 vTexCoord;

uniform vec4 uColor;
uniform sampler2D uTexture;
uniform bool uUseTexture;

out vec4 fragColor;

void main() {
    if (uUseTexture) {
        fragColor = texture(uTexture, vTexCoord) * uColor;
    } else {
        fragColor = uColor;
    }
}
)";

    if (!m_shader.build(vertShaderSrc, fragShaderSrc)) {
        LOGE("UIRenderer: Failed to compile UI shader");
        return false;
    }

    // Dynamic quad VAO/VBO setup
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    // Position attrib (0)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // TexCoord attrib (1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;
}

void UIRenderer::beginPass(int screenWidth, int screenHeight) {
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_shader.bind();
    m_shader.setVec2("uScreenSize", static_cast<float>(screenWidth), static_cast<float>(screenHeight));
}

void UIRenderer::drawQuad(float x, float y, float width, float height, const float color[4]) {
    m_shader.setVec4("uColor", color[0], color[1], color[2], color[3]);
    m_shader.setBool("uUseTexture", false);

    float x2 = x + width;
    float y2 = y + height;

    float vertices[] = {
        x,  y,  0.0f, 0.0f,
        x2, y,  1.0f, 0.0f,
        x,  y2, 0.0f, 1.0f,

        x2, y,  1.0f, 0.0f,
        x2, y2, 1.0f, 1.0f,
        x,  y2, 0.0f, 1.0f,
    };

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void UIRenderer::drawTexturedQuad(float x, float y, float width, float height, GLuint textureID,
                                 float u0, float v0, float u1, float v1, const float color[4]) {
    m_shader.setVec4("uColor", color[0], color[1], color[2], color[3]);
    m_shader.setBool("uUseTexture", true);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    m_shader.setInt("uTexture", 0);

    float x2 = x + width;
    float y2 = y + height;

    float vertices[] = {
        x,  y,  u0, v0,
        x2, y,  u1, v0,
        x,  y2, u0, v1,

        x2, y,  u1, v0,
        x2, y2, u1, v1,
        x,  y2, u0, v1,
    };

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void UIRenderer::endPass() {
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void UIRenderer::destroy() {
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    m_vao = 0;
    m_vbo = 0;
    m_shader.destroy();
}
