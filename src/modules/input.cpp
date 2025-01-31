#include "input.h"

#include <glfw3.h>

#include <functional>
#include <iostream>
#include <unordered_map>

#include "components/graphics.h"
#include "components/input.h"

using namespace std;
using namespace flux::components;

namespace flux::modules {

static std::unordered_map<KeyboardKey, KeyState> keyboard_events;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action,
                 int mods) {
  keyboard_events[static_cast<KeyboardKey>(key)] =
      static_cast<KeyState>(action);
};

InputSystem::InputSystem(flecs::world& world) {
  glfwSetKeyCallback(world.get<Window>()->ptr, KeyCallback);

  world.component<InputHandler>();
  world.set<Input>({keyboard_events});

  world.system<Input>().kind(flecs::PostLoad).each([](Input& input) {
    glfwPollEvents();
    input.keyboard_events = keyboard_events;
    keyboard_events.clear();
  });
}

}  // namespace flux::modules