#pragma once

#include <flecs.h>

namespace flux::modules {

#define LOG_BUFF_SIZE 1024u

struct Shaders {
  Shaders(flecs::world& world);
};

}  // namespace flux::modules
