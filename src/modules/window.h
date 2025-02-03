#pragma once

#include <glfw3.h>

struct Window {
  GLFWwindow* ptr;
  const GLFWvidmode* video_mode;
};