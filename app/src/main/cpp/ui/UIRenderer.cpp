#include "ui/UIRenderer.h"
#include "core/Log.h"

UIRenderer::~UIRenderer() {
    destroy();
}

bool UIRenderer::init(int screenWidth, int screenHeight) {
    onScreenChanged(screenWidth, screenHeight);

    const char* vertSrc = R"(#version 300 es
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTex;
layout(location = 2) in vec4 aColor;

uniform mat4 uOrtho;
out vec2 vTex;
out vec4 vColor;

void main() {
    gl_Position = uOrtho * vec4(aPos, 0.0, 1.0);
    vTex = aTex;
    vColor = aColor;
}
)";

    const char* fragSrc = R"(#version 300 es
precision mediump float;
in vec2 vTex;
in vec4 vColor;
uniform sampler2D uTex;
uniform bool uUseTex;
out vec4 fragColor;

void main() {
    if (uUseTex) {
        fragColor = texture(uTex, vTex) * vColor;
    } else {
        fragColor = vColor;
    }
}
)";

    if (!m_shader.build(vertSrc, fragSrc)) {
        LOGE("UIRenderer: Shader build failed");
        return false;
    }

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_verts), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)offsetof(QuadVertex, x));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)offsetof(QuadVertex, u));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)offsetof(QuadVertex, r));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    return true;
}

void UIRenderer::onScreenChanged(int w, int h) {
    m_screenW = std::max(1, w);
    m_screenH = std::max(1, h);
    buildOrtho();
}

void UIRenderer::buildOrtho() {
    float left = 0.0f, right = (float)m_screenW;
    float top = 0.0f, bottom = (float)m_screenH;
    float nearVal = -1.0f, farVal = 1.0f;

    m_ortho[0] = 2.0f / (right - left);  m_ortho[1] = 0.0f;                  m_ortho[2] = 0.0f;                     m_ortho[3] = 0.0f;
    m_ortho[4] = 0.0f;                  m_ortho[5] = 2.0f / (top - bottom);  m_ortho[6] = 0.0f;                     m_ortho[7] = 0.0f;
    m_ortho[8] = 0.0f;                  m_ortho[9] = 0.0f;                  m_ortho[10] = -2.0f / (farVal - nearVal); m_ortho[11] = 0.0f;
    m_ortho[12] = -(right + left)/(right - left); m_ortho[13] = -(top + bottom)/(top - bottom); m_ortho[14] = -(farVal + nearVal)/(farVal - nearVal); m_ortho[15] = 1.0f;
}

void UIRenderer::drawRect(float x, float y, float w, float h, float r, float g, float b, float a) {
    float uv[4] = {0,0,1,1};
    drawTexturedRect(x, y, w, h, uv, r, g, b, a, 0);
}

void UIRenderer::drawTexturedRect(float x, float y, float w, float h, float srcUV[4],
                                  float r, float g, float b, float a, GLuint textureId) {
    if (m_quadCount >= kMaxQuadsPerFlush || (m_currentTexture != textureId && m_quadCount > 0)) {
        flush();
    }
    m_currentTexture = textureId;

    int idx = m_quadCount * 4;
    float u0 = srcUV[0], v0 = srcUV[1], u1 = srcUV[2], v1 = srcUV[3];

    m_verts[idx + 0] = { x,     y,     u0, v0, r, g, b, a };
    m_verts[idx + 1] = { x + w, y,     u1, v0, r, g, b, a };
    m_verts[idx + 2] = { x + w, y + h, u1, v1, r, g, b, a };
    m_verts[idx + 3] = { x,     y + h, u0, v1, r, g, b, a };

    m_quadCount++;
}

void UIRenderer::flush() {
    if (m_quadCount == 0) return;

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_shader.bind();
    m_shader.setMat4("uOrtho", m_ortho);

    if (m_currentTexture != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_currentTexture);
        m_shader.setInt("uTex", 0);
        m_shader.setBool("uUseTex", true);
    } else {
        m_shader.setBool("uUseTex", false);
    }

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_quadCount * 4 * sizeof(QuadVertex), m_verts);

    for (int i = 0; i < m_quadCount; ++i) {
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
    }

    glBindVertexArray(0);
    m_quadCount = 0;
    m_currentTexture = 0;
}

void UIRenderer::destroy() {
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    m_vao = m_vbo = 0;
    m_shader.destroy();
}
