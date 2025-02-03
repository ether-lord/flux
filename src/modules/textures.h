#pragma once

#include <flecs.h>

#include <string>

namespace flux {

struct TextureData {
  std::string name;
};

struct Texture {
  unsigned int id;
};

struct Textures {
  Textures(flecs::world& world);
};

}  // namespace flux