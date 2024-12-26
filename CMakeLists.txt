cmake_minimum_required(VERSION 3.22)
project(SDL3-Template-Project)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_SUPPRESS_REGENERATION true) # disable ZERO_CHECK target creation

##########################################################################################
# SLD3
##########################################################################################

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_SHARED_LIBS   ON  CACHE BOOL "Build Raylib as shared library" FORCE)
set(BUILD_EXAMPLES      OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES         OFF CACHE BOOL "" FORCE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
    GIT_TAG "preview-3.1.6"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
    SDL3_image
    GIT_REPOSITORY "https://github.com/libsdl-org/SDL_image.git"
    GIT_TAG "preview-3.1.0"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SDL3_image)

##########################################################################################
# Project executable setup
##########################################################################################

add_executable(
    Threshold
    src/main.cpp
)

add_library(game SHARED
    src/game/export.cpp
)
set_target_properties(game PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    LINK_FLAGS "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/src/game/libgame.exports"
)


target_link_libraries(Threshold PUBLIC SDL3::SDL3 SDL3_image::SDL3_image)
target_link_libraries(game PUBLIC SDL3::SDL3 SDL3_image::SDL3_image)