#include "input.h"

#include <glfw3.h>

#include <functional>

#include "components/graphics.h"
#include "components/input.h"

using namespace flux::components;

namespace flux::modules {

void KeyCallback(GLFWwindow* window, int key, int scancode, int action,
                 int mods) {

};

InputSystem::InputSystem(flecs::world& world) {
  glfwSetInputMode(world.get<Window>()->ptr, GLFW_STICKY_KEYS, GLFW_TRUE);

  world.component<Input>();
  world.component<InputHandler>();

  world.system<const InputHandler>()
      .kind(flecs::PostLoad)
      .each([](flecs::entity e, const InputHandler& handler) {
        auto window = e.world().get<Window>();

        if (glfwGetKey(window->ptr, GLFW_KEY_ESCAPE) == GLFW_PRESS)
          e.set<Input>({KeyboardKey::kEscape, KeyState::kPressed});
        if (glfwGetKey(window->ptr, GLFW_KEY_W) == GLFW_PRESS)
          e.set<Input>({KeyboardKey::kKeyW, KeyState::kPressed});
      });
}

}  // namespace flux::modules