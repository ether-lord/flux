#include <iostream>
#include <vector>

#include "flecs.h"
#include "glm/glm.hpp"
#include "render2d.h"
#include "resources_manager.h"
#include "shader.h"
#include "shader_program.h"

using namespace std;
using namespace flecs;
using namespace glm;
using namespace flux;
using namespace flux::shader;
using namespace flux::resources;
using namespace flux::modules;

void process_input(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  auto monitor = glfwGetPrimaryMonitor();
  auto video_mode = glfwGetVideoMode(monitor);

  auto window = glfwCreateWindow(video_mode->width, video_mode->height, "FLUX",
                                 monitor, NULL);

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  glEnable(GL_DEPTH_TEST);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  world game;

  game.import <Buffer2d>();
  game.import <Render2d>();

  auto triangle = game.entity("triangle").add<Shape2d>();

  std::vector<glm::vec3> vertices = {
      {-0.5f, -0.5f, 0.f}, {0.0f, 0.5f, 0.f}, {0.5f, -0.5f, 0.f}};
  std::vector<glm::vec3> colors = {{0, 0, 1}, {0, 1, 0}, {1, 0, 0}};
  std::vector<unsigned int> indices = {0, 1, 2};

  triangle.set<Shape2d>(
      Shape2d{.vertices = vertices, .colors = colors, .indices = indices});

  // ===== Move to shader system =====
  auto shader_src = ResourcesManager::get().GetShaderSource("shape2d_vertex");
  auto vertex_shader = Shader(ShaderType::VERTEX, shader_src);
  vertex_shader.Compile();
  if (!vertex_shader.GetCompileStatus()) {
    cout << vertex_shader.GetInfoLog() << endl;
  }
  shader_src = ResourcesManager::get().GetShaderSource("shape2d_frag");
  auto frag_shader = Shader(ShaderType::FRAGMENT, shader_src);
  frag_shader.Compile();
  if (!frag_shader.GetCompileStatus()) {
    cout << frag_shader.GetInfoLog() << endl;
  }

  auto shader_program = ShaderProgram();
  shader_program.AttachShader(vertex_shader);
  shader_program.AttachShader(frag_shader);
  shader_program.Link();
  shader_program.Use();

  if (!shader_program.GetLinkStatus()) {
    cout << shader_program.GetInfoLog() << endl;
  }
  // ===== Move to shader system =====

  while (!glfwWindowShouldClose(window)) {
    process_input(window);

    glClearColor(0.009f, 0.195f, 0.0126f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    game.progress();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}

void process_input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}
