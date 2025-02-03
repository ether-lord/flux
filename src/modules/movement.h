#pragma once

#include <flecs.h>

namespace flux {

struct Direction {
  float x;
  float y;
  float z;
};

struct Speed {
  float magnitude;
};

struct Position {
  float x;
  float y;
  float z;
};

struct Movement {
  Movement(flecs::world& world);
};

}  // namespace flux
