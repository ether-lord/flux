#pragma once

#include <glfw3.h>

#include <unordered_map>
#include <glm/vec2.hpp>

namespace flux::components {

enum class KeyState {
  kNone = 0,
  kPressed = GLFW_PRESS,
  kReleased = GLFW_RELEASE,
  kRepeate = GLFW_REPEAT
};

enum class KeyboardKey {
  kEscape = GLFW_KEY_ESCAPE,
  kKeyW = GLFW_KEY_W,
  kKeyS = GLFW_KEY_S,
  kKeyD = GLFW_KEY_D,
  kKeyA = GLFW_KEY_A
};

struct InputTarget {};

struct Input {
  std::unordered_map<KeyboardKey, KeyState> keyboard_events;
  std::unordered_map<KeyboardKey, KeyState> keyboard_state;
  glm::vec2 mouse_offset;
};

}  // namespace flux::components
