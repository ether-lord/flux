#include "engine.h"

#include "glad.h"

using namespace flux::components;

namespace flux::engine {

Engine::Engine() {
  InitGlfw();
  CreateWindow();
  InitGlad();
}

void Engine::Start() {
  is_running = true;
  while (is_running) {
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
  auto id = entity::CreateEntityId();

  auto monitor = glfwGetPrimaryMonitor();
  auto video_mode = glfwGetVideoMode(monitor);

  auto glfw_window = glfwCreateWindow(video_mode->width, video_mode->height, "FLUX",
                                 monitor, NULL);

  auto win = components::Window{
    .window = glfw_window,
    .monitor = monitor,
    .mode = video_mode
  };
  
  glfwMakeContextCurrent(glfw_window);

  windows[id] = win;
}

}  // namespace flux::engine
