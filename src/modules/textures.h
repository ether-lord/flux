#pragma once

#include <flecs.h>

#include <string>
#include <unordered_map>

namespace flux {
  

struct DiffuseMap {
  std::string path;
  unsigned int id;
};

struct Textures {
  Textures(flecs::world& world);
};

struct LoadedTextures {
  std::unordered_map<std::string, unsigned int> texture_path_to_id;
};

}  // namespace flux