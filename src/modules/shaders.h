#pragma once

#define LOG_BUFF_SIZE 1024u

#include <flecs.h>

#include <string>
#include <unordered_map>

namespace flux {


struct Shader {
  std::string name;
  unsigned int id;
};

struct LoadedShaders {
  std::unordered_map<std::string, unsigned int> shader_name_to_id;
};

struct Shaders {
  Shaders(flecs::world& world);
};

}  // namespace flux
