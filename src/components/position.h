#pragma once

namespace flux::components {

struct Position {
  Position() : x(0.f), y(0.f), z(0.f) {};
  float x;
  float y;
  float z;
};

}  // namespace flux::component