#include "camera.h"

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(m_fov), m_aspect, m_nearPlane,
                            m_farPlane);
}
