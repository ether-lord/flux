#pragma once

#include <glfw3.h>

namespace flux::components {

enum class KeyState {
  kPressed = GLFW_PRESS
};

enum class KeyboardKey {
  kEscape = GLFW_KEY_ESCAPE,
  kKeyW = GLFW_KEY_W
};

struct InputHandler {};

struct Input {
  KeyboardKey key;
  KeyState state;
};

} // namespace flux::components
