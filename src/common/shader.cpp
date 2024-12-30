#include "shader.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

static auto readSource(std::string_view path) -> std::string {
    std::ifstream file(path.data());
    if (!file.is_open()) {
        throw std::runtime_error(std::string{"Failed to open shader file: "} +
                                 std::string(path));
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

Shader::Shader(ShaderKind kind, std::string_view path) {
    GLuint shaderType;
    switch (kind) {
    case ShaderKind::Vertex:
        shaderType = GL_VERTEX_SHADER;
        break;
    case ShaderKind::Fragment:
        shaderType = GL_FRAGMENT_SHADER;
        break;
    case ShaderKind::Geometry:
    default:
        shaderType = GL_GEOMETRY_SHADER;
        break;
    }
    GLuint const shader_id = glCreateShader(shaderType);
    std::string source = readSource(path);

    SDL_Log("PATH: %s", path.data());
    SDL_Log("SHADER: %s", source.data());

    const GLchar* source_ptr = source.data();
    GLint length = static_cast<GLint>(source.length());
    glShaderSource(shader_id, 1, &source_ptr, &length);
    glCompileShader(shader_id);

    GLint success = 0;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE) {
        // Get the log length
        GLint log_length = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

        if (log_length > 0) {
            // Allocate a buffer to hold the log
            std::string log_buffer(log_length, '\0');

            // Retrieve the shader info log
            glGetShaderInfoLog(shader_id, log_length, nullptr,
                               log_buffer.data());

            glDeleteShader(shader_id);
            throw std::runtime_error("shader compilation failed: " +
                                     log_buffer);
        }
        glDeleteShader(shader_id);
        throw std::runtime_error(
            "shader compilation failed but no log provided");
    }
}

Shader::~Shader() noexcept { glDeleteShader(m_id); }

Shader::Shader(Shader&& other) : m_id(other.m_id) {}

Shader& Shader::operator=(Shader&& other) {
    if (this->m_id == other.m_id) {
        return *this;
    }
    this->m_id = other.m_id;
    other.m_id = 0;
    return *this;
}

ShaderProgram::ShaderProgram(const Shader& vertexShader,
                             const Shader& fragmentShader,
                             std::optional<Shader> geometryShader) {
    m_id = glCreateProgram();

    glAttachShader(m_id, vertexShader.id());
    glAttachShader(m_id, fragmentShader.id());
    if (geometryShader.has_value()) {
        glAttachShader(m_id, geometryShader.value().id());
    }
    glLinkProgram(m_id);

    glDetachShader(m_id, vertexShader.id());
    glDetachShader(m_id, fragmentShader.id());
    if (geometryShader.has_value()) {
        glDetachShader(geometryShader.value().id(), m_id);
    }

    int success = 0;
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        // Handle program linking error
        GLint log_length = 0;
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &log_length);

        if (log_length > 0) {
            std::string log_buffer(log_length, '\0');
            glGetProgramInfoLog(m_id, log_length, nullptr, log_buffer.data());
            throw std::runtime_error("shader program linking failed: " +
                                     log_buffer);
        }
        throw std::runtime_error(
            "shader program linking failed but no log provided");
    }

    glValidateProgram(m_id);
}

ShaderProgram::~ShaderProgram() noexcept { glDeleteProgram(m_id); }

ShaderProgram::ShaderProgram(ShaderProgram&& other) : m_id(other.m_id) {}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) {
    if (this->m_id == other.m_id) {
        return *this;
    }
    this->m_id = other.m_id;
    other.m_id = 0;
    return *this;
}