#pragma once

#include <SDL3/SDL_opengl.h>
#include <glm/vec3.hpp>
#include <string_view>
#include <vector>

namespace StlLoader {
struct Result {
    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;
};
Result load(std::string_view path);
} // namespace StlLoader