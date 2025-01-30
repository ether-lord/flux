#pragma once

#include <glfw3.h>

namespace flux::components {

enum class KeyState {
  kPressed = GLFW_PRESS
};

enum class KeyboardKey {
  kEscape = GLFW_KEY_ESCAPE
};

struct InputHandler {};

struct Input {
  KeyboardKey key;
  KeyState state;
};

} // namespace flux::components
