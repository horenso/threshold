#include "mesh.h"
#include "shader.h"
#include "stlloader.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Mesh::Mesh(std::string_view path, const ShaderProgram& shaderProgram) noexcept
    : m_shaderProgram(shaderProgram) {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);

    auto result = StlLoader::load(path);
    m_vertices = std::move(result.vertices);
    m_indices = std::move(result.indices);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 m_vertices.size() * sizeof(decltype(m_vertices)::value_type),
                 m_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 m_indices.size() * sizeof(decltype(m_indices)::value_type),
                 m_indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          sizeof(decltype(m_vertices)::value_type), 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::render(const glm::mat4& transform) noexcept {
    glUseProgram(this->m_shaderProgram.id());

    glUniformMatrix4fv(
        m_shaderProgram.getUniform(ShaderKind::Vertex, "transfrom"), 1,
        GL_FALSE, glm::value_ptr(transform));

    glBindVertexArray(this->m_vao);
    // TODO save mesh data together with m_vao in mesh
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glUseProgram(0);
}