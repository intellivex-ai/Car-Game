#pragma once
#include <GLES3/gl3.h>
#include <Jolt/Jolt.h>
#include <Jolt/Math/Mat44.h>
#include <vector>
#include <cstdint>

struct Vertex {
    float position[3];
    float normal[3];
    float texCoord[2];
};

class Mesh {
public:
    Mesh() = default;
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    bool upload(const std::vector<Vertex>& vertices,
                const std::vector<uint16_t>& indices);

    void draw() const;
    void draw(const JPH::Mat44& worldTransform) const { (void)worldTransform; draw(); }

    void destroy();

    bool isValid() const { return m_vao != 0; }
    uint32_t indexCount() const { return m_indexCount; }

    static Mesh createBox   (float w, float h, float d);
    static Mesh createPlane (float w, float d, int subdivisionsX = 1, int subdivisionsZ = 1);
    static Mesh createCylinder(float radius, float height, int segments = 16);

private:
    GLuint   m_vao        = 0;
    GLuint   m_vbo        = 0;
    GLuint   m_ebo        = 0;
    uint32_t m_indexCount = 0;
};
