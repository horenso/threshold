#include "SDL3/SDL_init.h"
#ifdef __linux__
// Use default visibility for exported symbols
#define EXPORT __attribute__((visibility("default")))
#else
#define EXPORT ""
#endif

#include <iostream>

struct Game {};

extern "C" {
EXPORT SDL_AppResult gameTick(Game* game) {
    std::cout << std::unitbuf;
    std::cout << "2";
    return SDL_APP_CONTINUE;
}
}
