#include "object.h"

#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/gtx/quaternion.hpp>

#include <SDL3/SDL.h>

Object::Object(const glm::vec3& pos, const glm::quat& rot)
    : m_position(pos), m_rotation(rot) {}

glm::mat4 Object::getTransformationMatrix() const {
    glm::mat4 rotationMatrix = glm::toMat4(m_rotation);
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1), m_position);
    return translationMatrix * rotationMatrix;
}

void Object::rotate(const glm::vec3& axis, float angle) {
    glm::quat deltam_rotation = glm::angleAxis(angle, glm::normalize(axis));
    m_rotation = glm::normalize(deltam_rotation * m_rotation);
}

void Object::move(const glm::vec3& deltaPosition) {
    m_position += deltaPosition;
}
