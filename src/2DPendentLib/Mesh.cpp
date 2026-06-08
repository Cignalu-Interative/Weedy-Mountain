#include "pch.h"
#include "Mesh.h"
#include <SDL.h>

Mesh::~Mesh()
{
    Destroy();
}

void Mesh::Upload(const std::vector<float>& p_vertices,
    const std::vector<VertexAttrib>& p_attribs,
    const std::vector<unsigned int>& p_indices)
{
    Destroy(); // reset si déjà uploadé

    // --- VBO ---
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER,
        p_vertices.size() * sizeof(float),
        p_vertices.data(),
        GL_STATIC_DRAW);

    // --- VAO ---
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);  // bind VBO dans le VAO

    // Calculer le stride total (somme de tous les counts)
    int totalStride = 0;
    for (const auto& attrib : p_attribs)
        totalStride += attrib.count;
    totalStride *= sizeof(float);

    // Configurer chaque attribut
    size_t offset = 0;
    for (const auto& attrib : p_attribs)
    {
        glVertexAttribPointer(attrib.location, attrib.count,
            GL_FLOAT, GL_FALSE,
            totalStride, (void*)offset);
        glEnableVertexAttribArray(attrib.location);
        offset += attrib.count * sizeof(float);
    }

    // --- EBO (optionnel) ---
    if (!p_indices.empty())
    {
        glGenBuffers(1, &m_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            p_indices.size() * sizeof(unsigned int),
            p_indices.data(),
            GL_STATIC_DRAW);
        m_indexCount = (int)p_indices.size();
    }

    // Calculer le nb de vertices
    if (totalStride > 0)
        m_vertexCount = (int)(p_vertices.size() * sizeof(float)) / totalStride;

    // Dé-binder proprement
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if (m_ebo) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::Draw() const
{
    glBindVertexArray(m_vao);
    if (m_indexCount > 0)
        glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
    else
        glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
}

void Mesh::Destroy()
{
    if (m_vao) { glDeleteVertexArrays(1, &m_vao); m_vao = 0; }
    if (m_vbo) { glDeleteBuffers(1, &m_vbo);      m_vbo = 0; }
    if (m_ebo) { glDeleteBuffers(1, &m_ebo);      m_ebo = 0; }
    m_vertexCount = m_indexCount = 0;
}