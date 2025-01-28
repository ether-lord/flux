#pragma once

#include <flecs.h>

namespace flux::modules {

#define LOG_BUFF_SIZE 1024u

struct Buffer2d {
  Buffer2d(flecs::world& world);
};

struct ShaderLoader {
  ShaderLoader(flecs::world& world);
};

}  // namespace flux::modules
