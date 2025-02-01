#pragma once

#include <glfw3.h>

#include <unordered_map>

namespace flux::components {

enum class KeyState { kPressed = GLFW_PRESS, kReleased = GLFW_RELEASE };

enum class KeyboardKey {
  kEscape = GLFW_KEY_ESCAPE,
  kKeyW = GLFW_KEY_W,
  kKeyS = GLFW_KEY_S,
  kKeyD = GLFW_KEY_D,
  kKeyA = GLFW_KEY_A
};

struct InputTarget { };

struct Input {
  std::unordered_map<KeyboardKey, KeyState> keyboard_events;
  glm::vec2 mouse_offset;
};

}  // namespace flux::components
