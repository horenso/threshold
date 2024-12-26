#include "SDL3/SDL_init.h"
#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <csignal>
#include <dlfcn.h>
#include <filesystem>
#include <iostream>
#include <time.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

static SDL_FRect mouseposrect;

static void* gameDynamicLibraryHandle = nullptr;

static bool reload = false;

static std::string libraryPath =
    (std::filesystem::current_path() / "libgame.so").string();

using GameTickFunc = SDL_AppResult (*)(void*);
static GameTickFunc gameTick = nullptr;

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

    error = dlerror();
    if (error) {
        std::cerr << "Failed to load symbol: " << error << std::endl;
        exit(1);
    }
}

SDL_AppResult SDL_AppIterate(void* appstate) {
    sleep(1);
    if (reload) {
        loadGameLibrary();
        reload = false;
    }
    return gameTick(appstate);
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    switch (event->type) {
    case SDL_EVENT_QUIT: /* triggers on last window close and other things. End
                            the program. */
        return SDL_APP_SUCCESS;

    case SDL_EVENT_KEY_DOWN: /* quit if user hits ESC key */
        if (event->key.scancode == SDL_SCANCODE_ESCAPE) {
            return SDL_APP_FAILURE;
        }
        break;

    case SDL_EVENT_MOUSE_MOTION: /* keep track of the latest mouse position */
        /* center the square where the mouse is */
        mouseposrect.x = event->motion.x - (mouseposrect.w / 2);
        mouseposrect.y = event->motion.y - (mouseposrect.h / 2);
        break;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    std::cout << std::unitbuf;
    std::signal(SIGUSR1, sigintHandler);

    try {
        loadGameLibrary();
    } catch (...) {
        SDL_Log("Failed to load game library");
        return SDL_APP_FAILURE;
    }

    SDL_SetAppMetadata("SDL Hello World Example", "1.0",
                       "com.example.sdl-hello-world");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init(SDL_INIT_VIDEO) failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    window = SDL_CreateWindow("Hello SDL", 640, 480, 0);
    if (!window) {
        SDL_Log("SDL_CreateWindow() failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("SDL_CreateRenderer() failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_SetRenderVSync(renderer, 1)) {
        SDL_Log("Could not enable VSync! SDL error: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    mouseposrect.x = mouseposrect.y =
        -1000; /* -1000 so it's offscreen at start */
    mouseposrect.w = mouseposrect.h = 50;

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}