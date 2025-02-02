#include "input_handling.h"

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

static bool first_mouse_callback = true;

static glm::vec2 mouse_position = {0.f, 0.f};
static glm::vec2 mouse_offset = {0.f, 0.f};

static float mouse_sensitivity = 0.1f;

static std::unordered_map<KeyboardKey, KeyState> keyboard_events;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action,
                 int mods) {
  keyboard_events[static_cast<KeyboardKey>(key)] =
      static_cast<KeyState>(action);
};

void MouseCallback(GLFWwindow* window, double mouse_x, double mouse_y) {
  if (first_mouse_callback) {
    first_mouse_callback = false;
    return;
  }

  float offset_x = mouse_x - mouse_position.x;
  float offset_y = mouse_position.y - mouse_y;

  offset_x *= mouse_sensitivity;
  offset_y *= mouse_sensitivity;

  mouse_offset = {offset_x, offset_y};

  mouse_position = {static_cast<float>(mouse_x), static_cast<float>(mouse_y)};
};

InputHandling::InputHandling(flecs::world& world) {
  world.component<InputTarget>();

  auto window = world.get<Window>();
  glfwSetKeyCallback(window->ptr, KeyCallback);
  glfwSetCursorPosCallback(window->ptr, MouseCallback);

  mouse_position = {window->video_mode->width / 2.f,
                    window->video_mode->height / 2.f};

  // Registering keyboard keys
  std::unordered_map<KeyboardKey, KeyState> keyboard_state;
  keyboard_state[KeyboardKey::kEscape] = KeyState::kNone;
  keyboard_state[KeyboardKey::kKeyW] = KeyState::kNone;
  keyboard_state[KeyboardKey::kKeyA] = KeyState::kNone;
  keyboard_state[KeyboardKey::kKeyS] = KeyState::kNone;
  keyboard_state[KeyboardKey::kKeyD] = KeyState::kNone;

  world.set<Input>(
      {.keyboard_events = keyboard_events, .keyboard_state = keyboard_state});

  world.system<Input>("InputSystem")
      .kind(flecs::PostLoad)
      .each([](flecs::entity e, Input& input) {
        auto window = e.world().get<Window>()->ptr;

        glfwPollEvents();
        input.keyboard_events = keyboard_events;
        input.mouse_offset = mouse_offset;
        keyboard_events.clear();
        mouse_offset = glm::vec2{0.f, 0.f};

        for (auto& [key, state] : input.keyboard_state) {
          state = static_cast<KeyState>(glfwGetKey(window, static_cast<int>(key)));
        }
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