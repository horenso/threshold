#include "util.h"

#include <SDL3/SDL_log.h>
#include <glm/glm.hpp>

void printMat4(const glm::mat4& matrix) {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            SDL_Log("%f ", matrix[col][row]);
        }
        SDL_Log("\n");
    }
    SDL_Log("\n");
}
