#include "object.h"

#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/gtx/quaternion.hpp>

#include <SDL3/SDL.h>

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
    // Log the new position after the move
    SDL_Log("Camera moved by (%f, %f, %f). New position: (%f, %f, %f)",
            deltaPosition.x, deltaPosition.y, deltaPosition.z, m_position.x,
            m_position.y, m_position.z);
}

void Object::setPosition(const glm::vec3& newPosition) {
    m_position = newPosition;
    // Log the new position after setting
    SDL_Log("Camera position set to (%f, %f, %f)", m_position.x, m_position.y,
            m_position.z);
}
