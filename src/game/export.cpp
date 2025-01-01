#include "config.h"
#include "gamedata.h"
#include "mesh.h"
#include "shader.h"

#include <SDL3/SDL_log.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_stdinc.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string_view>

#ifdef __linux__
// Use default visibility for exported symbols
#define EXPORT __attribute__((visibility("default")))
#else
#define EXPORT ""
#endif

static Shader* vertexShader{nullptr};
static Shader* fragmentShader{nullptr};
static ShaderProgram* shaderProgram{nullptr};
static Mesh* mesh1{nullptr};
static Mesh* mesh2{nullptr};

static glm::mat4* transform{nullptr};

extern "C" {
EXPORT SDL_AppResult gameInit(GameData* gameData) {
    vertexShader =
        new Shader{ShaderKind::Vertex,
                   "/home/jannis/coding/threshold/src/shaders/shader1.vert"};
    fragmentShader =
        new Shader{ShaderKind::Fragment,
                   "/home/jannis/coding/threshold/src/shaders/shader1.frag"};
    shaderProgram =
        new ShaderProgram{*vertexShader, *fragmentShader, std::nullopt};
    mesh1 = new Mesh{"/home/jannis/coding/threshold/src/stls/suzanne.stl",
                     *shaderProgram};
    mesh2 = new Mesh{"/home/jannis/coding/threshold/src/stls/donut.stl",
                     *shaderProgram};

    transform = new glm::mat4(1.0f);

    SDL_Log("loaded...");

    glViewport(0, 0, Config::width, Config::height);
    return SDL_APP_CONTINUE;
}

EXPORT SDL_AppResult gameTick(GameData* gameData) {
    Uint64 start_time = SDL_GetTicks();

    glClearColor(1.0f, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mesh1->render(*transform);
    mesh2->render(*transform);

    SDL_GL_SwapWindow(gameData->window);

    if (Config::logFrameTime) {
        Uint64 frame_time = SDL_GetTicks() - start_time;
        SDL_Log("frame_time: %lu", frame_time);
    }

    return SDL_APP_CONTINUE;
}

EXPORT SDL_AppResult gameInput(GameData* gameData, SDL_Event* event) {
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;

    case SDL_EVENT_KEY_DOWN:
        if (event->key.scancode == SDL_SCANCODE_ESCAPE) {
            return SDL_APP_SUCCESS;
        } else if (event->key.scancode == SDL_SCANCODE_W) {
            mesh1->move(glm::vec3(0.05f, 0.05f, 0.0f));
            SDL_Log("moved");
        }
        break;

    case SDL_EVENT_MOUSE_MOTION:
        break;
    }
    return SDL_APP_CONTINUE;
}
}
