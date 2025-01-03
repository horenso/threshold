#include "mesh.h"

#include "stlloader.h"
#include "util.h"

#include <SDL3/SDL_log.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Mesh::Mesh(std::string_view path, const ShaderProgram& shaderProgram) noexcept
    : m_shaderProgram(shaderProgram) {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo); // Vertex buffer for positions
    glGenBuffers(1, &m_nbo); // Normal buffer
    glGenBuffers(1, &m_ebo); // Element buffer for indices

    glBindVertexArray(m_vao);

    auto result = StlLoader::load(path);
    m_vertices = std::move(result.vertices);
    m_normals = std::move(result.normals);
    m_indices = std::move(result.indices);

    // Bind and upload vertex position data
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * 3 * sizeof(GLfloat),
                 m_vertices.data(), GL_STATIC_DRAW);

    // Bind and upload normal data
    // glBindBuffer(GL_ARRAY_BUFFER, m_nbo);
    // glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(GLfloat),
    //              m_normals.data(), GL_STATIC_DRAW);

    // Bind and upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint),
                 m_indices.data(), GL_STATIC_DRAW);

    // Vertex position attribute (index 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    SDL_Log("sizeof(decltype(m_vertices)::value_type): %u",
            sizeof(decltype(m_vertices)::value_type));

    // Normal attribute (index 1)
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::render(const Camera& camera) noexcept {
    glUseProgram(this->m_shaderProgram.id());

    glm::mat4 modelMatrix = getTransformationMatrix();
    glm::mat4 viewMatrix = camera.getTransformationMatrix();
    glm::mat4 projectionMatrix = camera.getProjectionMatrix();

    glUniformMatrix4fv(m_shaderProgram.getUniform("uModel"), 1, GL_FALSE,
                       glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(m_shaderProgram.getUniform("uTransform"), 1, GL_FALSE,
                       glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(m_shaderProgram.getUniform("uProjection"), 1, GL_FALSE,
                       glm::value_ptr(projectionMatrix));

    glBindVertexArray(this->m_vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}
