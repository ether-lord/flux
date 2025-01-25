cmake_minimum_required(VERSION 3.16)

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
    -fno-sanitize=alignment)
  add_compile_options(-fsanitize=address, -lasan, -fsanitize=undefined)
  add_link_options(-fsanitize=address, -fsanitize=undefined)
endif()

project(${PROJECT_NAME})

find_package(OpenGL REQUIRED)

add_subdirectory(lib/glad)
add_subdirectory(lib/glfw)
add_subdirectory(lib/json)

set(
  ENGINE_SOURCES
  src/resources_manager.cpp src/resources_manager.h
  src/shader.cpp src/shader.h
  src/shader_program.cpp src/shader_program.h
  src/system_manager.cpp src/system_manager.h
  src/types.cpp src/types.h
)

add_executable(${PROJECT_NAME} main.cpp ${ENGINE_SOURCES})


target_include_directories(
  ${PROJECT_NAME} PUBLIC
  src
  lib/glad/include/glad
  lib/glfw/include/GLFW
  lib/json/include/nlohmann
  lib/stb
)

target_link_libraries(
  ${PROJECT_NAME} PUBLIC
  ${CMAKE_DL_LIBS} glad glfw OpenGL::GL
  nlohmann_json::nlohmann_json
)
