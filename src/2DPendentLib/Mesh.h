#pragma once
#include "glad/glad.h"
#include <vector>

// Décrit un attribut dans le VBO
struct VertexAttrib
{
    unsigned int location;  // location dans le shader (layout location = X)
    int          count;     // nb de composantes (2 pour vec2, 3 pour vec3, etc.)
    // type implicitement float ici, tu peux étendre avec GLenum type si besoin
};

class Mesh
{
public:
    Mesh() = default;
    ~Mesh();

    // Upload des données vers le GPU
    // p_attribs décrit le layout des vertices (position, UV, normal, etc.)
    void Upload(const std::vector<float>& p_vertices,
        const std::vector<VertexAttrib>& p_attribs,
        const std::vector<unsigned int>& p_indices = {});

    void Draw() const;
    void Destroy();

    bool HasIndices() const { return m_indexCount > 0; }
    int  VertexCount() const { return m_vertexCount; }

private:
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    unsigned int m_ebo = 0;
    int          m_vertexCount = 0;
    int          m_indexCount = 0;
};