#pragma once

#include <flecs.h>

#define GLFW_INCLUDE_NONE

#include "glad.h"
#include "glfw3.h"

namespace flux::modules {

struct Render {
  Render(flecs::world& world);
};

struct WindowPreProcessing {
  WindowPreProcessing(flecs::world& world);
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

}  // namespace flux::modules
