cmake_minimum_required(VERSION 3.22)

set(PROJECT_NAME flux)
set(CMAKE_CXX_STANDARD 17)

set(JSON_BuildTests OFF CACHE INTERNAL "")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(
    -fsanitize=address
    -fsanitize=undefined
    -fno-sanitize-recover=all
    -fsanitize=float-divide-by-zero
    -fsanitize=float-cast-overflow
    -fno-sanitize=null
    -fno-sanitize=alignment
    -Ofast)
  add_compile_options(-fsanitize=address, -lasan, -fsanitize=undefined)
  add_link_options(-fsanitize=address, -fsanitize=undefined)
endif()

project(${PROJECT_NAME})

include(FetchContent)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG bf71a834948186f4097caa076cd2663c69a10e1e
)

FetchContent_Declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp.git
  GIT_TAG v5.4.3
)

FetchContent_Declare(
  flecs
  GIT_REPOSITORY https://github.com/SanderMertens/flecs.git
  GIT_TAG 41a14e2dd41c2ff2092cc8512ab1a35f93f6d9fd
)

FetchContent_MakeAvailable(glm)
FetchContent_MakeAvailable(assimp)
FetchContent_MakeAvailable(flecs)

find_package(OpenGL REQUIRED)

add_subdirectory(lib/glad)
add_subdirectory(lib/glfw)
add_subdirectory(lib/json)

set(
  ENGINE_SOURCES

  src/resources_manager.cpp

  src/modules/render.cpp
  src/modules/resources.cpp
  src/modules/input.cpp
  src/modules/camera.cpp
  src/modules/movement.cpp
)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)

add_executable(${PROJECT_NAME} main.cpp ${ENGINE_SOURCES})

target_include_directories(
  ${PROJECT_NAME} PUBLIC
  src
  src/components
  src/modules
  lib/glad/include/glad
  lib/glfw/include/GLFW
  lib/json/include/nlohmann
  lib/stb
)

target_link_libraries(
  ${PROJECT_NAME} PUBLIC
  ${CMAKE_DL_LIBS} glad glfw OpenGL::GL
  nlohmann_json::nlohmann_json glm::glm assimp
  flecs::flecs_static
)
