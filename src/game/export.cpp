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
static Camera* camera{nullptr};

extern "C" {
EXPORT SDL_AppResult gameInit(GameData* gameData) {
    vertexShader = new Shader{ShaderKind::Vertex, "shaders/shader1.vert"};
    fragmentShader = new Shader{ShaderKind::Fragment, "shaders/shader1.frag"};
    shaderProgram =
        new ShaderProgram{*vertexShader, *fragmentShader, std::nullopt};
    mesh1 = new Mesh{"stls/suzanne.stl", *shaderProgram};
    mesh2 = new Mesh{"stls/donut.stl", *shaderProgram};
    camera = new Camera{};
    camera->setPosition({0, 0, 0});

    mesh2->move({1, 1, 0});

    SDL_Log("loaded...");

    glViewport(0, 0, Config::width, Config::height);
    return SDL_APP_CONTINUE;
}

EXPORT SDL_AppResult gameTick(GameData* gameData) {
    Uint64 start_time = SDL_GetTicks();

    glClearColor(0x18, 0x18, 0x18, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mesh1->render(*camera);
    mesh2->render(*camera);

    SDL_GL_SwapWindow(gameData->window);

    if (Config::logFrameTime) {
        Uint64 frame_time = SDL_GetTicks() - start_time;
        SDL_Log("frame_time: %lu", frame_time);
    }

    return SDL_APP_CONTINUE;
}

EXPORT SDL_AppResult gameInput(GameData* gameData, SDL_Event* event) {
    const float moveSpeed = 0.05f;    // Speed of movement (you can tweak this)
    const float rotationSpeed = 0.5f; // Speed of rotation (you can tweak this)

    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;

    case SDL_EVENT_KEY_DOWN:
        // Check for movement keys (W, A, S, D)
        if (event->key.scancode == SDL_SCANCODE_W) {
            camera->move(glm::vec3(0.0f, 0.0f, -moveSpeed)); // Move forward
            SDL_Log("Moved Forward");
        } else if (event->key.scancode == SDL_SCANCODE_S) {
            camera->move(glm::vec3(0.0f, 0.0f, moveSpeed)); // Move backward
            SDL_Log("Moved Backward");
        } else if (event->key.scancode == SDL_SCANCODE_A) {
            camera->move(glm::vec3(-moveSpeed, 0.0f, 0.0f)); // Move left
            SDL_Log("Moved Left");
        } else if (event->key.scancode == SDL_SCANCODE_D) {
            camera->move(glm::vec3(moveSpeed, 0.0f, 0.0f)); // Move right
            SDL_Log("Moved Right");
        }

        // Check for rotation keys (Arrow Keys)
        else if (event->key.scancode == SDL_SCANCODE_UP) {
            camera->rotate(glm::vec3(1.0f, 0.0f, 0.0f),
                           rotationSpeed); // Rotate up
            SDL_Log("Rotated Up");
        } else if (event->key.scancode == SDL_SCANCODE_DOWN) {
            camera->rotate(glm::vec3(1.0f, 0.0f, 0.0f),
                           -rotationSpeed); // Rotate down
            SDL_Log("Rotated Down");
        } else if (event->key.scancode == SDL_SCANCODE_LEFT) {
            camera->rotate(glm::vec3(0.0f, 1.0f, 0.0f),
                           rotationSpeed); // Rotate left
            SDL_Log("Rotated Left");
        } else if (event->key.scancode == SDL_SCANCODE_RIGHT) {
            camera->rotate(glm::vec3(0.0f, 1.0f, 0.0f),
                           -rotationSpeed); // Rotate right
            SDL_Log("Rotated Right");
        }

        // Check for Escape key (to quit)
        else if (event->key.scancode == SDL_SCANCODE_ESCAPE) {
            return SDL_APP_SUCCESS;
        }
        break;

    case SDL_EVENT_MOUSE_MOTION:
        // Handle mouse motion (optional, for looking around)
        break;
    }

    return SDL_APP_CONTINUE;
}
}
