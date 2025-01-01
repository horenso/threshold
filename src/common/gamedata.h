#pragma once

#include "mesh.h"
#include <SDL3/SDL.h>

#define GL_GLEXT_PROTOTYPES 1
#include <SDL3/SDL_opengl.h>

struct GameData {
    SDL_Window* window{nullptr};
    SDL_GLContext context{nullptr};
};
