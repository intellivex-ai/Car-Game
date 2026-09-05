// Mesh.cpp — Phase 3 implementation placeholder.
// Full VAO/VBO/EBO upload and primitive factories implemented in Phase 3.
#include "renderer/Mesh.h"
#include "core/Log.h"

Mesh::~Mesh() { destroy(); }

Mesh::Mesh(Mesh&& other) noexcept
    : m_vao(other.m_vao), m_vbo(other.m_vbo),
      m_ebo(other.m_ebo), m_indexCount(other.m_indexCount) {
    other.m_vao = other.m_vbo = other.m_ebo = 0;
    other.m_indexCount = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        destroy();
        m_vao = other.m_vao; m_vbo = other.m_vbo;
        m_ebo = other.m_ebo; m_indexCount = other.m_indexCount;
        other.m_vao = other.m_vbo = other.m_ebo = 0;
        other.m_indexCount = 0;
    }
    return *this;
}

bool Mesh::upload(const std::vector<Vertex>& /*vertices*/,
                  const std::vector<uint16_t>& /*indices*/) {
    LOGI("Mesh::upload — stub (Phase 3)");
    return false; // TODO Phase 3
}

void Mesh::draw() const {
    if (m_vao) {
        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indexCount),
                       GL_UNSIGNED_SHORT, nullptr);
        glBindVertexArray(0);
    }
}

void Mesh::destroy() {
    if (m_vao) { glDeleteVertexArrays(1, &m_vao); m_vao = 0; }
    if (m_vbo) { glDeleteBuffers(1, &m_vbo); m_vbo = 0; }
    if (m_ebo) { glDeleteBuffers(1, &m_ebo); m_ebo = 0; }
    m_indexCount = 0;
}

Mesh Mesh::createBox(float /*w*/, float /*h*/, float /*d*/) {
    return Mesh(); // TODO Phase 3
}

Mesh Mesh::createPlane(float /*w*/, float /*d*/, int /*sx*/, int /*sz*/) {
    return Mesh(); // TODO Phase 3
}

Mesh Mesh::createCylinder(float /*radius*/, float /*height*/, int /*segments*/) {
    return Mesh(); // TODO Phase 3
}
