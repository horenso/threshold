#include "stlloader.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string_view>
#include <vector>

StlLoader::Result StlLoader::load(std::string_view path) {
    std::ifstream file(path.data(), std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }

    file.seekg(80); // Skip the header (80 bytes)

    uint32_t triangle_count;
    file.read(reinterpret_cast<char*>(&triangle_count), sizeof(triangle_count));
    if (!file) {
        throw std::runtime_error("Failed to read triangle count");
    }

    std::vector<glm::vec3> vertices;

    // For each triangle
    for (uint32_t i = 0; i < triangle_count; ++i) {
        file.seekg(12, std::ios::cur); // Skip the normal vector (12 bytes)

        for (int j = 0; j < 3; ++j) {
            glm::vec3 vertex;
            file.read(reinterpret_cast<char*>(&vertex),
                      sizeof(vertex)); // Read directly into glm::vec3
            if (!file) {
                throw std::runtime_error("Failed to read vertex data");
            }
            vertices.push_back(vertex);
        }

        file.seekg(2, std::ios::cur); // Skip attribute byte count (2 bytes)
    }

    std::cout << "Triangle count: " << triangle_count << std::endl;

    std::vector<GLuint> indices(vertices.size());
    for (size_t i = 0; i < indices.size(); ++i) {
        indices[i] = static_cast<unsigned int>(i);
    }

    return Result{vertices, indices};
}