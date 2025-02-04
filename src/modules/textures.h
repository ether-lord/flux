#pragma once

#include <flecs.h>

#include <string>
#include <unordered_map>

namespace flux {

struct Texture {
  std::string name;
};

struct TextureHandle {
  unsigned int id;
};

struct Textures {
  Textures(flecs::world& world);
};

struct LoadedTextures {
  std::unordered_map<std::string, unsigned int> texture_path_to_id;
};

}  // namespace flux