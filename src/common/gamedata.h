#pragma once

#include "mesh.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

struct GameData {
    SDL_Window* window{nullptr};
    SDL_GLContext context{nullptr};
    bool loaded{false};
};
