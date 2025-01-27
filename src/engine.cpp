#include "engine.h"

namespace flux::engine {

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

Engine::Engine() {
  InitGlfw();
  CreateWindow();
  InitGlad();

  glEnable(GL_DEPTH_TEST);
  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Engine::Start() {
  is_running = true;
  while (is_running) {
    process_input(window_);

    glClearColor(0.009f, 0.195f, 0.0126f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    is_running = !glfwWindowShouldClose(window_);

    glfwSwapBuffers(window_);
    glfwPollEvents();
  }
}

void Engine::InitGlfw() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Engine::InitGlad() { gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); }

void Engine::CreateWindow() {
  auto monitor = glfwGetPrimaryMonitor();
  auto video_mode = glfwGetVideoMode(monitor);

  window_ = glfwCreateWindow(video_mode->width, video_mode->height, "FLUX",
                             monitor, NULL);

  glfwMakeContextCurrent(window_);
}

}  // namespace flux::engine
