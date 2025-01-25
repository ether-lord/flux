#pragma once

#include <unordered_map>

#define GLFW_INCLUDE_NONE
#include "entity.h"
#include "glad.h"
#include "glfw3.h"

namespace flux::engine {

void process_input(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

class Engine {
 public:
  Engine();

  void Start();

  bool is_running = false;

 private:
  void InitGlfw();
  void InitGlad();
  void CreateWindow();

  GLFWwindow* window_;
};

}  // namespace flux::engine
