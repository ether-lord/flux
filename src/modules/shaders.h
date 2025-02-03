#pragma once

#define LOG_BUFF_SIZE 1024u

#include <flecs.h>

#include <string>

namespace flux {

struct ShaderData {
  std::string name;
};

struct Shader {
  unsigned int id;
};

struct Shaders {
  Shaders(flecs::world& world);
};

}  // namespace flux
