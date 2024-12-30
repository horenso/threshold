#include "config.h"
#include "gamedata.h"
#include "mesh.h"
#include "shader.h"

#define GL_GLEXT_PROTOTYPES 1
#include "SDL3/SDL.h"
#include "SDL3/SDL_stdinc.h"

#include <optional>
#include <string_view>
#include <utility>

#ifdef __linux__
// Use default visibility for exported symbols
#define EXPORT __attribute__((visibility("default")))
#else
#define EXPORT ""
#endif

static Shader* vertexShader{nullptr};
static Shader* fragmentShader{nullptr};
static ShaderProgram* shaderProgram{nullptr};
static Mesh* mesh{nullptr};

void load(GameData* gameData) {
    vertexShader =
        new Shader{ShaderKind::Vertex,
                   "/home/jannis/coding/threshold/src/shaders/shader1.vert"};
    fragmentShader =
        new Shader{ShaderKind::Fragment,
                   "/home/jannis/coding/threshold/src/shaders/shader1.frag"};
    shaderProgram =
        new ShaderProgram{*vertexShader, *fragmentShader, std::nullopt};
    mesh = new Mesh{"/home/jannis/coding/threshold/src/stls/suzanne.stl",
                    *shaderProgram};
    SDL_Log("loaded...");
}

extern "C" {
EXPORT SDL_AppResult gameTick(GameData* gameData) {
    if (!gameData->loaded) {
        load(gameData);
        gameData->loaded = true;
    }
    glViewport(0, 0, Config::width, Config::height);
    glClearColor(255, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    mesh->render();
    SDL_GL_SwapWindow(gameData->window);
    return SDL_APP_CONTINUE;
}

EXPORT SDL_AppResult gameInput(GameData* gameData, SDL_Event* event) {
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;

    case SDL_EVENT_KEY_DOWN:
        if (event->key.scancode == SDL_SCANCODE_ESCAPE) {
            return SDL_APP_FAILURE;
        }
        break;

    case SDL_EVENT_MOUSE_MOTION:
        break;
    }
    return SDL_APP_CONTINUE;
}
}
