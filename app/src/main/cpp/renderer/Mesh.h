#pragma once
#include <GLES3/gl3.h>
#include <vector>
#include <cstdint>

/**
 * Vertex layout used for all 3D geometry.
 *
 * Position:  3 floats (x, y, z)
 * Normal:    3 floats (nx, ny, nz)
 * TexCoord:  2 floats (u, v)
 * Total:     8 floats = 32 bytes per vertex
 */
struct Vertex {
    float position[3];
    float normal[3];
    float texCoord[2];
};

/**
 * Mesh
 *
 * Holds a VAO, VBO, and EBO on the GPU.
 * Supports indexed rendering (glDrawElements).
 *
 * All geometry is uploaded once at construction; meshes are immutable after upload.
 *
 * Factory functions create common primitive shapes for placeholder rendering:
 *   Mesh::createBox()
 *   Mesh::createPlane()
 *   Mesh::createCylinder()
 */
class Mesh {
public:
    Mesh() = default;
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    // Move allowed for storage in containers
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    /**
     * Uploads vertex and index data to the GPU.
     * @param vertices  Array of Vertex structs
     * @param indices   Array of uint16 triangle indices
     */
    bool upload(const std::vector<Vertex>& vertices,
                const std::vector<uint16_t>& indices);

    /** Draws the mesh (assumes shader is bound). */
    void draw() const;

    void destroy();

    bool isValid() const { return m_vao != 0; }
    uint32_t indexCount() const { return m_indexCount; }

    // --- Primitive factories ---
    static Mesh createBox   (float w, float h, float d);
    static Mesh createPlane (float w, float d, int subdivisionsX = 1, int subdivisionsZ = 1);
    static Mesh createCylinder(float radius, float height, int segments = 16);

private:
    GLuint   m_vao        = 0;
    GLuint   m_vbo        = 0;
    GLuint   m_ebo        = 0;
    uint32_t m_indexCount = 0;
};
