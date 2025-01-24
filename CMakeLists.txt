cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME flux)
set(CMAKE_CXX_STANDARD 17)

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

add_definitions(-DGLFW_INCLUDE_NONE)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(
  ${PROJECT_NAME} PRIVATE
  lib/glad/include/glad
  lib/glfw/include/GLFW
)

target_link_libraries(
  ${PROJECT_NAME}
  ${CMAKE_DL_LIBS}
  glad
  glfw
  OpenGL::GL
)
