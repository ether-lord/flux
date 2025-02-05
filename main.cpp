#include <flecs.h>

#define GLFW_INCLUDE_NONE

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <random>
#include <vector>

#include "modules/camera.h"
#include "modules/input.h"
#include "modules/movement.h"
#include "modules/render.h"
#include "modules/textures.h"
#include "modules/window.h"

using namespace std;
using namespace flecs;
using namespace glm;
using namespace flux;

int main() {
  world game;

  game.import <flecs::stats>();
  game.set<flecs::Rest>({});

  game.import <Render>();

  Transform transform = {.position = {1.f, 0.f, 0.f},
                         .rotation = {0.f, 0.f, 0.f}};

  Mesh dirt_block_data;
  dirt_block_data.vertices = {
      // front side : 0-3
      {.position = {-0.5f, 0.5f, 0.5f}, .normal = {0.f,0.f,1.f}, .uv = {0.f, 1.f}},
      {.position = {0.5f, 0.5f, 0.5f}, .normal = {0.f,0.f,1.f}, .uv = {1.f, 1.f}},
      {.position = {0.5f, -0.5f, 0.5f}, .normal = {0.f,0.f,1.f}, .uv = {1.f, 0.f}},
      {.position = {-0.5f, -0.5f, 0.5f}, .normal = {0.f,0.f,1.f}, .uv = {0.f, 0.f}},

      // back side : 4-7
      {.position = {-0.5f, 0.5f, -0.5f}, .normal = {0.f,0.f, -1.f}, .uv = {0.f, 1.f}},
      {.position = {0.5f, 0.5f, -0.5f}, .normal = {0.f,0.f, -1.f}, .uv = {1.f, 1.f}},
      {.position = {0.5f, -0.5f, -0.5f}, .normal = {0.f,0.f, -1.f}, .uv = {1.f, 0.f}},
      {.position = {-0.5f, -0.5f, -0.5f}, .normal = {0.f,0.f, -1.f}, .uv = {0.f, 0.f}},

      // top side : 8-11
      {.position = {-0.5f, 0.5f, 0.5f}, .normal = {0.f,1.f,0.f}, .uv = {0.f, 0.f}},
      {.position = {0.5f, 0.5f, 0.5f}, .normal = {0.f,1.f,0.f}, .uv = {1.f, 0.f}},
      {.position = {0.5f, 0.5f, -0.5f}, .normal = {0.f,1.f,0.f}, .uv = {1.f, 1.f}},
      {.position = {-0.5f, 0.5f, -0.5f}, .normal = {0.f,1.f,0.f}, .uv = {0.f, 1.f}},

      // bottom side : 12-15
      {.position = {-0.5f, -0.5f, 0.5f}, .normal = {0.f,-1.f,-0.f}, .uv = {0.f, 0.f}},
      {.position = {0.5f, -0.5f, 0.5f}, .normal = {0.f,-1.f,-0.f}, .uv = {1.f, 0.f}},
      {.position = {0.5f, -0.5f, -0.5f}, .normal = {0.f,-1.f,-0.f}, .uv = {1.f, 1.f}},
      {.position = {-0.5f, -0.5f, -0.5f}, .normal = {0.f,-1.f,-0.f}, .uv = {0.f, 1.f}},

      // left side : 16-19
      {.position = {-0.5f, -0.5f, -0.5f}, .normal = {-1.f, 0.f,0.f}, .uv = {0.f, 0.f}},
      {.position = {-0.5f, -0.5f, 0.5f}, .normal = {-1.f, 0.f,0.f}, .uv = {1.f, 0.f}},
      {.position = {-0.5f, 0.5f, 0.5f}, .normal = {-1.f, 0.f,0.f}, .uv = {1.f, 1.f}},
      {.position = {-0.5f, 0.5f, -0.5f}, .normal = {-1.f, 0.f,0.f}, .uv = {0.f, 1.f}},

      // right side : 20-23
      {.position = {0.5f, -0.5f, -0.5f}, .normal = {1.f,0.f,0.f}, .uv = {0.f, 0.f}},
      {.position = {0.5f, -0.5f, 0.5f}, .normal = {1.f,0.f,0.f}, .uv = {1.f, 0.f}},
      {.position = {0.5f, 0.5f, 0.5f}, .normal = {1.f,0.f,0.f}, .uv = {1.f, 1.f}},
      {.position = {0.5f, 0.5f, -0.5f}, .normal = {1.f,0.f,0.f}, .uv = {0.f, 1.f}},
  };

  dirt_block_data.indices = {
      0,  1,  2,  2,  3,  0,   // front side
      4,  5,  6,  6,  7,  4,   // back side
      8,  9,  10, 10, 11, 8,   // top side
      12, 13, 14, 14, 15, 12,  // bottom side
      16, 17, 18, 18, 19, 16,  // bottom side
      20, 21, 22, 22, 23, 20   // right side
  };

  auto dirt_block = game.prefab("Dirt");
  dirt_block.set<Mesh>(dirt_block_data);
  dirt_block.set<Texture>({"dirt.png"});
  dirt_block.set<Transform>(transform);

  static std::default_random_engine e;
  static std::uniform_real_distribution<> dis(0, 20);

  for (int i = 0; i < 250; ++i) {
    auto dirt = game.entity().is_a(dirt_block);

    dirt.get_mut<Transform>()->position.x += dis(e) - 10;
    dirt.get_mut<Transform>()->position.y += dis(e) - 10;
    dirt.get_mut<Transform>()->position.z += dis(e) - 10;
  }

  auto window = game.get<Window>();
  game.entity<Window>().add<InputTarget>();

  mat4 projection = mat4(1.0f);
  projection =
      perspective(radians(70.0f),
                  (float)window->video_mode->width / window->video_mode->height,
                  0.1f, 100.0f);
  game.set<Projection>({projection});

  while (!glfwWindowShouldClose(window->ptr)) {
    game.progress();

    glfwSwapBuffers(window->ptr);
  }

  return 0;
}
