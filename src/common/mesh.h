#pragma once

#include "glm/fwd.hpp"
#include "shader.h"

#define GL_GLEXT_PROTOTYPES 1
#include <SDL3/SDL_opengl.h>

#include <glm/glm.hpp>

#include <string_view>
#include <vector>

class Mesh final {
    public:
    explicit Mesh(std::string_view path,
                  const ShaderProgram& shaderProgram) noexcept;

    void render(const glm::mat4& transfrom) noexcept;

    private:
    std::vector<glm::vec3> m_vertices;
    std::vector<GLuint> m_indices;
    const ShaderProgram& m_shaderProgram;
    GLuint m_vao{0};
    GLuint m_vbo{0};
    GLuint m_ebo{0};
};