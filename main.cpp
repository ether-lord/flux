#include <iostream>
#include <vector>

#include "components/shader.h"
#include "flecs.h"
#include "glm/glm.hpp"
#include "graphics.h"
#include "render.h"
#include "resources.h"
#include "resources_manager.h"

using namespace std;
using namespace flecs;
using namespace glm;

using namespace flux::components;
using namespace flux::resources;
using namespace flux::modules;

int main() {
  world game;

  game.import <WindowPreProcessing>();
  game.import <ShaderLoader>();
  game.import <Buffer2d>();
  game.import <Render>();

  auto triangle = game.entity("triangle").add<Shape2d>();

  std::vector<glm::vec3> vertices = {
      {-0.5f, -0.5f, 0.f}, {0.0f, 0.5f, 0.f}, {0.5f, -0.5f, 0.f}};
  std::vector<glm::vec3> colors = {{0, 0, 1}, {0, 1, 0}, {1, 0, 0}};
  std::vector<unsigned int> indices = {0, 1, 2};

  triangle.set<Shape2d>(
      Shape2d{.vertices = vertices, .colors = colors, .indices = indices});

  ShaderInfo vertex_shader_info{GL_VERTEX_SHADER, "shape2d_vertex"};
  ShaderInfo frag_shader_info{GL_FRAGMENT_SHADER, "shape2d_frag"};
  ShaderData color_shader_data{{vertex_shader_info, frag_shader_info}};

  auto color_shader = game.entity("ColorShader");
  color_shader.add<Shader>();
  color_shader.set<ShaderData>(color_shader_data);

  auto window = game.get<Window>();
  while (!glfwWindowShouldClose(window->ptr)) {
    game.progress();

    glfwSwapBuffers(window->ptr);
    glfwPollEvents();
  }

  return 0;
}
