#include "renderer/Mesh.h"
#include "core/Log.h"

Mesh::~Mesh() {
    destroy();
}

Mesh::Mesh(Mesh&& other) noexcept
    : m_vao(other.m_vao), m_vbo(other.m_vbo), m_ebo(other.m_ebo), m_indexCount(other.m_indexCount) {
    other.m_vao = other.m_vbo = other.m_ebo = 0;
    other.m_indexCount = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        destroy();
        m_vao = other.m_vao;
        m_vbo = other.m_vbo;
        m_ebo = other.m_ebo;
        m_indexCount = other.m_indexCount;
        other.m_vao = other.m_vbo = other.m_ebo = 0;
        other.m_indexCount = 0;
    }
    return *this;
}

bool Mesh::upload(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices) {
    destroy();
    m_indexCount = static_cast<uint32_t>(indices.size());

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint16_t), indices.data(), GL_STATIC_DRAW);

    // Position (0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    // Normal (1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    // TexCoord (2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    return true;
}

void Mesh::draw() const {
    if (m_vao != 0) {
        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_SHORT, 0);
        glBindVertexArray(0);
    }
}

void Mesh::destroy() {
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_ebo) glDeleteBuffers(1, &m_ebo);
    m_vao = m_vbo = m_ebo = 0;
    m_indexCount = 0;
}

Mesh Mesh::createBox(float w, float h, float d) {
    Mesh mesh;
    float hw = w * 0.5f, hh = h * 0.5f, hd = d * 0.5f;

    std::vector<Vertex> verts = {
        {{-hw, -hh,  hd}, {0, 0, 1}, {0, 0}},
        {{ hw, -hh,  hd}, {0, 0, 1}, {1, 0}},
        {{ hw,  hh,  hd}, {0, 0, 1}, {1, 1}},
        {{-hw,  hh,  hd}, {0, 0, 1}, {0, 1}},

        {{ hw, -hh, -hd}, {0, 0, -1}, {0, 0}},
        {{-hw, -hh, -hd}, {0, 0, -1}, {1, 0}},
        {{-hw,  hh, -hd}, {0, 0, -1}, {1, 1}},
        {{ hw,  hh, -hd}, {0, 0, -1}, {0, 1}}
    };

    std::vector<uint16_t> idx = {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7
    };

    mesh.upload(verts, idx);
    return mesh;
}

Mesh Mesh::createPlane(float w, float d, int subdivisionsX, int subdivisionsZ) {
    Mesh mesh;
    float hw = w * 0.5f, hd = d * 0.5f;

    std::vector<Vertex> verts = {
        {{-hw, 0.0f, -hd}, {0, 1, 0}, {0, 0}},
        {{ hw, 0.0f, -hd}, {0, 1, 0}, {1, 0}},
        {{ hw, 0.0f,  hd}, {0, 1, 0}, {1, 1}},
        {{-hw, 0.0f,  hd}, {0, 1, 0}, {0, 1}}
    };

    std::vector<uint16_t> idx = { 0, 1, 2, 0, 2, 3 };
    mesh.upload(verts, idx);
    return mesh;
}

Mesh Mesh::createCylinder(float radius, float height, int segments) {
    return createBox(radius * 2.0f, height, radius * 2.0f);
}
