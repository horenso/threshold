#include <glad/gl.h>

#define GL_GLEXT_PROTOTYPES 1
#include "SDL3/SDL_video.h"

#include "SDL3/SDL_init.h"
#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_video.h>

#include "config.h"
#include "gamedata.h"

#include <csignal>
#include <dlfcn.h>
#include <filesystem>
#include <iostream>

static GameData gameData{};

static void* gameDynamicLibraryHandle = nullptr;
static bool reload = false;

static std::string libraryPath =
    (std::filesystem::current_path() / "libgame.so").string();

using GameTickFunc = SDL_AppResult (*)(GameData*);
using GameInputFunc = SDL_AppResult (*)(GameData*, SDL_Event* event);
static GameTickFunc gameTick = nullptr;
static GameInputFunc gameInput = nullptr;

void sigintHandler(int) {
    std::cerr << "Reloading..." << std::endl;
    reload = true;
}

// Function to load the game library
void loadGameLibrary() {
    // Reset dlerror
    const char* error = dlerror();

    if (gameDynamicLibraryHandle) {
        dlclose(gameDynamicLibraryHandle);
        error = dlerror();
        if (error) {
            std::cerr << "Failed to close library: " << error << std::endl;
            exit(1);
        }
        gameDynamicLibraryHandle = nullptr;
        std::cout << "library unloaded\n";
    }

    gameDynamicLibraryHandle = dlopen(libraryPath.c_str(), RTLD_NOW);
    error = dlerror();
    if (error) {
        std::cerr << "Failed to open library: " << error << std::endl;
        exit(1);
    }

    gameTick = reinterpret_cast<GameTickFunc>(
        dlsym(gameDynamicLibraryHandle, "gameTick"));
    gameInput = reinterpret_cast<GameInputFunc>(
        dlsym(gameDynamicLibraryHandle, "gameInput"));

    error = dlerror();
    if (error) {
        std::cerr << "Failed to load symbol: " << error << std::endl;
        exit(1);
    }
}

void onFramebufferSizeChanged(void*, int width, int height) {
    glViewport(0, 0, width, height);
}

SDL_AppResult SDL_AppIterate(void*) {
    if (reload) {
        loadGameLibrary();
        reload = false;
    }
    return gameTick(&gameData);
}

SDL_AppResult SDL_AppEvent(void*, SDL_Event* event) {
    return gameInput(&gameData, event);
}

SDL_AppResult SDL_AppInit(void**, int, char*[]) {
    std::signal(SIGUSR1, sigintHandler);

    SDL_SetAppMetadata("SDL Hello World Example", "1.0",
                       "com.example.sdl-hello-world");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init(SDL_INIT_VIDEO) failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    auto* window = SDL_CreateWindow("Hello SDL", Config::width, Config::height,
                                    SDL_WINDOW_OPENGL);
    if (!window) {
        SDL_Log("SDL_CreateWindow() failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        SDL_Log("Failed to create OpenGL context: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress)) {
        SDL_Log("Failed to initialize GLAD");
        SDL_GL_DestroyContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return SDL_APP_FAILURE;
    }

    try {
        loadGameLibrary();
    } catch (...) {
        SDL_Log("Failed to load game library");
        return SDL_APP_FAILURE;
    }

    if (!SDL_GL_SetSwapInterval(1)) {
        SDL_Log("Failed to enable VSync: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    glViewport(0, 0, Config::width, Config::height);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    gameData = {
        .window = window,
        .context = context,
        .loaded = false,
    };

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void*, SDL_AppResult) {
    SDL_GL_DestroyContext(gameData.context);
    SDL_DestroyWindow(gameData.window);
    SDL_Quit();
}