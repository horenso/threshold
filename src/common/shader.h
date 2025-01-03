#pragma once

#define GL_GLEXT_PROTOTYPES 1
#include <SDL3/SDL_opengl.h>

#include <optional>
#include <string_view>

enum class ShaderKind {
    Vertex,
    Fragment,
    Geometry,
};

class Shader final {
    public:
    ~Shader() noexcept;

    Shader(ShaderKind kind, std::string_view path);

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&&);
    Shader& operator=(Shader&&);

    auto id() const -> GLuint { return m_id; }

    private:
    explicit Shader(GLuint id) : m_id(id) {}

    GLuint m_id{0};
};

class ShaderProgram final {
    public:
    explicit ShaderProgram(const Shader& vertexShader,
                           const Shader& fragmentShader,
                           std::optional<Shader> geometryShader);
    ~ShaderProgram() noexcept;

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    ShaderProgram(ShaderProgram&&);
    ShaderProgram& operator=(ShaderProgram&&);

    auto id() const -> GLuint { return m_id; }
    auto getUniform(std::string_view name) const -> GLuint;

    private:
    GLuint m_id{0};
};
