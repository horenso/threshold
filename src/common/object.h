#pragma once

#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Object {
    public:
    Object(const glm::vec3& pos = glm::vec3(0.0f),
           const glm::quat& rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f));

    glm::mat4 getTransformationMatrix() const;
    void rotate(const glm::vec3& axis, float angle);
    void move(const glm::vec3& deltaPosition);

    private:
    glm::vec3 m_position{};
    glm::quat m_rotation{};
};
