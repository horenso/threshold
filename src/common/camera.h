#pragma once

#define GL_GLEXT_PROTOTYPES 1
#include <SDL3/SDL_opengl.h>

#include "config.h"
#include "object.h"

class Camera : public Object {
    public:
    glm::mat4 getProjectionMatrix() const;

    private:
    GLfloat m_fov{45.0f};
    GLfloat m_aspect{Config::width / Config::height};
    GLfloat m_nearPlane{0.01f};
    GLfloat m_farPlane{1000.0f};
};
