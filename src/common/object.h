#pragma once

#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Object {
    public:
    Object() {}

    glm::mat4 getTransformationMatrix() const;
    void rotate(const glm::vec3& axis, float angle);
    void move(const glm::vec3& deltaPosition);
    void setPosition(const glm::vec3& newPosition);

    private:
    glm::vec3 m_position{};
    glm::quat m_rotation{};
};
