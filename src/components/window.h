#pragma once

#define GLFW_INCLUDE_NONE
#include "glfw3.h"

namespace flux::components {

struct Window {
  GLFWwindow* window;
  GLFWmonitor* monitor;
  const GLFWvidmode* mode;
};

}  // namespace flux::components