#include <flecs.h>

#define GLFW_INCLUDE_NONE

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "components/graphics.h"
#include "components/input.h"
#include "components/shader.h"
#include "modules/input.h"
#include "modules/render.h"
#include "modules/resources.h"
#include "modules/camera.h"
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
  game.import <InputSystem>();
  game.import <Camera>();
  game.import <Render>();

  Mesh square_data;
  square_data.vertices = {
      Vertex{.position{-0.5f, 0.5f, 0.f}, .uv = {0.f, 1.f}},
      Vertex{.position{0.5f, 0.5f, 0.f}, .uv = {1.f, 1.f}},
      Vertex{.position{0.5f, -0.5f, 0.f}, .uv = {1.f, 0.f}},
      Vertex{.position{-0.5f, -0.5f, 0.f}, .uv = {0.f, 0.f}}};
  square_data.indices = {0, 1, 2, 2, 3, 0};

  Transform transform = {.position = {1.f, 0.f, 0.f},
                         .rotation = {90.f, 90.f, 0.f}};

  auto square = game.entity("Square");
  square.set<Mesh>(square_data);
  square.set<Texture>({"container.jpg"});
  square.set<Transform>(transform);

  ShaderInfo vertex_shader_info{GL_VERTEX_SHADER, "vertex"};
  ShaderInfo frag_shader_info{GL_FRAGMENT_SHADER, "fragment"};
  ShaderData shader_data{{vertex_shader_info, frag_shader_info}};

  auto shader = game.entity("BasicShader");
  shader.add<Shader>();
  shader.set<ShaderData>(shader_data);

  auto window = game.get<Window>();
  game.entity<Window>().add<InputHandler>();

  mat4 projection = mat4(1.0f);
  projection =
      perspective(radians(45.0f),
                  (float)window->video_mode->width / window->video_mode->height,
                  0.1f, 100.0f);
  game.set<Projection>({projection});

  while (!glfwWindowShouldClose(window->ptr)) {
    game.progress();

    glfwSwapBuffers(window->ptr);
  }

  return 0;
}
