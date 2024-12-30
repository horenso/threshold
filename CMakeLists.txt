cmake_minimum_required(VERSION 3.22)
project(SDL3-Template-Project)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_SUPPRESS_REGENERATION true)

set(CLANG_EXTRA_WARNING_FLAGS
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wnon-virtual-dtor
        -Woverloaded-virtual
        -Wnull-dereference
)

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
# GLAD
##########################################################################################

FetchContent_Declare(
    glad
    GIT_REPOSITORY "https://github.com/Dav1dde/glad"
    GIT_TAG        "v2.0.6"
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)

FetchContent_GetProperties(glad)
if(NOT glad_POPULATED)
    message("Fetching glad")
    FetchContent_MakeAvailable(glad)

    add_subdirectory("${glad_SOURCE_DIR}/cmake" glad_cmake)
    glad_add_library(glad REPRODUCIBLE EXCLUDE_FROM_ALL LOADER API gl:core=4.6 EXTENSIONS GL_ARB_bindless_texture GL_EXT_texture_compression_s3tc)
endif()

##########################################################################################
# GL Math
##########################################################################################

include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY "https://github.com/g-truc/glm.git"
	GIT_TAG        "1.0.1"
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)

FetchContent_MakeAvailable(glm)

##########################################################################################
# Project executable setup
##########################################################################################

add_executable(
    Threshold
    src/main/main.cpp
)

add_library(game SHARED
    src/game/export.cpp
)
add_library(common STATIC
    src/common/stlloader.cpp
    src/common/shader.cpp
    src/common/mesh.cpp
)
set_target_properties(game PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    LINK_FLAGS "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/src/game/libgame.exports"
)

target_include_directories(Threshold PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/common)
target_include_directories(game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/common)

target_link_libraries(common PUBLIC
    SDL3::SDL3
    SDL3_image::SDL3_image
    glad
    glm::glm
)
target_link_libraries(Threshold PUBLIC 
    SDL3::SDL3
    SDL3_image::SDL3_image
    glad
    glm::glm
    common
)
target_link_libraries(game PUBLIC
    SDL3::SDL3
    SDL3_image::SDL3_image
    glad
    glm::glm
    common
)

target_compile_options(common PRIVATE ${CLANG_EXTRA_WARNING_FLAGS})
target_compile_options(game PRIVATE ${CLANG_EXTRA_WARNING_FLAGS})
target_compile_options(Threshold PRIVATE ${CLANG_EXTRA_WARNING_FLAGS})