#include <flecs.h>

#define GLFW_INCLUDE_NONE

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "components/input.h"
#include "modules/camera.h"
#include "modules/input_handling.h"
#include "modules/movement.h"
#include "modules/render.h"
#include "modules/window.h"
#include "modules/textures.h"

using namespace std;
using namespace flecs;
using namespace glm;

using namespace flux;
using namespace flux::components;
using namespace flux::modules;

int main() {
  world game;

  game.import <flecs::stats>();
  game.set<flecs::Rest>({});

  game.import <Buffering>();
  game.import <Movement>();
  game.import <Render>();
  game.import <InputHandling>();

  Transform transform = {.position = {1.f, 0.f, 0.f},
                         .rotation = {0.f, 0.f, 0.f}};

  Mesh dirt_block_data;
  dirt_block_data.vertices = {
      {.position = {-0.5f, 0.5f, 0.f}, .uv = {0.f, 1.f}},
      {.position = {0.5f, 0.5f, 0.f}, .uv = {1.f, 1.f}},
      {.position = {0.5f, -0.5f, 0.f}, .uv = {1.f, 0.f}},
      {.position = {-0.5f, -0.5f, 0.f}, .uv = {0.f, 0.f}}};

  dirt_block_data.indices = {0, 1, 2, 2, 3, 0};

  auto dirt_block = game.entity("Dirt");
  dirt_block.set<Mesh>(dirt_block_data);
  dirt_block.set<TextureData>({"dirt.png"});
  dirt_block.set<Transform>(transform);

  auto window = game.get<Window>();
  game.entity<Window>().add<InputTarget>();

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
