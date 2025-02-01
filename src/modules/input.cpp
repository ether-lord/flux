#include "modules/input.h"

#include <glfw3.h>

#include <functional>
#include <iostream>
#include <unordered_map>

#include "components/graphics.h"
#include "components/input.h"
#include "components/movement.h"

using namespace std;
using namespace flux::components;

namespace flux::modules {

struct MousePosition {
  float x, y;
};

static MousePosition mouse_position = {0, 0};
static float mouse_sensitivity = 0.1f;

static std::unordered_map<KeyboardKey, KeyState> keyboard_events;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action,
                 int mods) {
  keyboard_events[static_cast<KeyboardKey>(key)] =
      static_cast<KeyState>(action);
};

void MouseCallback(GLFWwindow* window, double mouse_x, double mouse_y) {
  float offset_x = mouse_x - mouse_position.x;
  float offset_y = mouse_y - mouse_position.y;

  mouse_position = {.x = static_cast<float>(mouse_x),
                    .y = static_cast<float>(mouse_y)};
};

InputHandling::InputHandling(flecs::world& world) {
  auto window = world.get<Window>();
  glfwSetKeyCallback(window->ptr, KeyCallback);
  glfwSetCursorPosCallback(window->ptr, MouseCallback);
  mouse_position = {window->video_mode->width / 2.f,
                    window->video_mode->height / 2.f};

  world.component<InputTarget>();
  world.set<Input>({keyboard_events});

  world.system<Input>("InputSystem")
      .kind(flecs::PostLoad)
      .each([](Input& input) {
        glfwPollEvents();
        input.keyboard_events = keyboard_events;
        keyboard_events.clear();
      });

  world.system<Window, const InputTarget>("Window input system")
      .kind(flecs::PostLoad)
      .each([](Window& window, const InputTarget) {
        if (keyboard_events.count(KeyboardKey::kEscape) &&
            keyboard_events[KeyboardKey::kEscape] == KeyState::kReleased) {
          glfwSetWindowShouldClose(window.ptr, true);
        }
      });

}

}  // namespace flux::modules