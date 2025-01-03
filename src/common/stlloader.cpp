#include "stlloader.h"

#include <SDL3/SDL_log.h>

#include <glm/geometric.hpp>

#include <fstream>
#include <set>
#include <stdexcept>
#include <string_view>
#include <vector>

// Custom comparator for glm::vec3
struct Vec3Comparator {
    bool operator()(const glm::vec3& a, const glm::vec3& b) const {
        if (a.x != b.x) {
            return a.x < b.x;
        }
        if (a.y != b.y) {
            return a.y < b.y;
        }
        return a.z < b.z;
    }
};

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
    SDL_Log("Triangle count: %u", triangle_count);

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> face_normals;
    std::vector<glm::vec3> vertex_normals;

    std::set<glm::vec3, Vec3Comparator> unique_vertices;
    std::vector<GLuint> indices;

    // For each triangle
    for (size_t i = 0; i < triangle_count; ++i) {
        glm::vec3 normal;
        file.read(reinterpret_cast<char*>(&normal), sizeof(normal));
        if (!file) {
            throw std::runtime_error("Failed to read vertex data");
        }
        face_normals.push_back(normal);

        for (int j = 0; j < 3; ++j) {
            glm::vec3 vertex;
            file.read(reinterpret_cast<char*>(&vertex),
                      sizeof(vertex)); // Read directly into glm::vec3
            if (!file) {
                throw std::runtime_error("Failed to read vertex data");
            }
            unique_vertices.emplace(vertex);
            indices.push_back(std::distance(unique_vertices.begin(),
                                            unique_vertices.find(vertex)));
        }

        file.seekg(2, std::ios::cur); // Skip attribute byte count (2 bytes)
    }
    file.peek();
    if (!file.eof()) {
        throw std::runtime_error(
            "STL file not EOF despite all triangles read!");
    }

    // for (size_t i = 0; i < triangle_count; ++i) {
    //     glm::vec3 normal = face_normals[i];
    //     for (size_t j = 0; j < 3; ++j) {
    //         size_t vertex_index = i * 3 + j;
    //         vertex_normals[vertex_index] += normal;
    //     }
    // }
    // for (auto& normal : vertex_normals) {
    //     normal = glm::normalize(normal);
    // }

    SDL_Log("total vertices: %zu unique: %zu", vertices.size(),
            unique_vertices.size());

    return Result{
        .vertices = {unique_vertices.begin(), unique_vertices.end()},
        .normals = vertex_normals,
        .indices = indices,
    };
}
