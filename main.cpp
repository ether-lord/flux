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
  game.import <TextureLoader>();
  game.import <Buffering>();
  game.import <Render>();

  Mesh square_data;
  square_data.vertices = {
      Vertex{.position{-0.5f, 0.5f, 0.f}, .uv = {0.f, 1.f}},
      Vertex{.position{0.5f, 0.5f, 0.f}, .uv = {1.f, 1.f}},
      Vertex{.position{0.5f, -0.5f, 0.f}, .uv = {1.f, 0.f}},
      Vertex{.position{-0.5f, -0.5f, 0.f}, .uv = {0.f, 0.f}}};
  square_data.indices = {0, 1, 2, 2, 3, 0};

  auto square = game.entity("Square");
  square.set<Mesh>(square_data);
  square.set<Texture>({"container.jpg"});

  ShaderInfo vertex_shader_info{GL_VERTEX_SHADER, "vertex"};
  ShaderInfo frag_shader_info{GL_FRAGMENT_SHADER, "fragment"};
  ShaderData shader_data{{vertex_shader_info, frag_shader_info}};

  auto shader = game.entity("BasicShader");
  shader.add<Shader>();
  shader.set<ShaderData>(shader_data);

  auto window = game.get<Window>();
  while (!glfwWindowShouldClose(window->ptr)) {
    game.progress();

    glfwSwapBuffers(window->ptr);
    glfwPollEvents();
  }

  return 0;
}
