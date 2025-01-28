#include <iostream>
#include <vector>

#include "flecs.h"
#include "glm/glm.hpp"
#include "graphics.h"
#include "render.h"
#include "resources.h"
#include "resources_manager.h"
#include "shader.h"
#include "shader_program.h"

using namespace std;
using namespace flecs;
using namespace glm;

using namespace flux::components;
using namespace flux::shader;
using namespace flux::resources;
using namespace flux::modules;

int main() {
  world game;

  game.import <WindowPreProcessing>();
  game.import <Buffer2d>();
  game.import <Render>();

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

  auto window = game.get<Window>();
  while (!glfwWindowShouldClose(window->ptr)) {
    game.progress();

    glfwSwapBuffers(window->ptr);
    glfwPollEvents();
  }

  return 0;
}
