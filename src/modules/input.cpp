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

static std::unordered_map<KeyboardKey, KeyState> keyboard_events;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action,
                 int mods) {
  keyboard_events[static_cast<KeyboardKey>(key)] =
      static_cast<KeyState>(action);
};

InputHandling::InputHandling(flecs::world& world) {
  glfwSetKeyCallback(world.get<Window>()->ptr, KeyCallback);

  world.component<InputHandler>();
  world.set<Input>({keyboard_events});

  world.system<Input>("Poll input events")
      .kind(flecs::PostLoad)
      .each([](Input& input) { glfwPollEvents(); });

  world.system<Window, const InputHandler>("Window input system")
      .kind(flecs::PostLoad)
      .each([](Window& window, const InputHandler) {
        if (keyboard_events.count(KeyboardKey::kEscape) &&
            keyboard_events[KeyboardKey::kEscape] == KeyState::kReleased) {
          glfwSetWindowShouldClose(window.ptr, true);
        }
      });

  world.system<Direction, const Speed, const InputHandler>("Movable objects input system")
      .kind(flecs::PostLoad)
      .each([&](flecs::entity e, Direction& direction, const Speed,
                const InputHandler&) {
        for (auto [key, state] : keyboard_events) {
          switch (state) {
            case KeyState::kPressed:
              if (key == KeyboardKey::kKeyW)
                direction.z = -1.f;
              else if (key == KeyboardKey::kKeyS)
                direction.z = 1.f;
              break;
            case KeyState::kReleased:
              direction = Direction{0.f, 0.f, 0.f};
              break;
            default:
              break;
          }
        }
      });

  world.system<Input>("Clear input events")
      .kind(flecs::PostLoad)
      .each([](Input& input) { keyboard_events.clear(); });
}

}  // namespace flux::modules