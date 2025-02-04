#pragma once

#include <flecs.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <unordered_map>

namespace flux {

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
  kKeyA = GLFW_KEY_A,
  kKeyShift = GLFW_KEY_LEFT_SHIFT
};

struct InputTarget {};

struct InputData {
  std::unordered_map<KeyboardKey, KeyState> keyboard_events;
  std::unordered_map<KeyboardKey, KeyState> keyboard_state;
  glm::vec2 mouse_offset;
};

struct Input {
  Input(flecs::world& world);
};

}  // namespace flux::modules
