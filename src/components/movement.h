#pragma once

#include <glm/glm.hpp>

namespace flux::components {

struct Direction {
  float x;
  float y;
  float z;
};

struct Speed {
  float value;
};

struct Position {
  float x;
  float y;
  float z;
};

}  // namespace flux::components
