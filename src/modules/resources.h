#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <flecs.h>

#include <assimp/Importer.hpp>

namespace flux::modules {

#define LOG_BUFF_SIZE 1024u

struct Buffering {
  Buffering(flecs::world& world);
};

struct ShaderLoader {
  ShaderLoader(flecs::world& world);
};

struct TextureLoader {
  TextureLoader(flecs::world& world);
};

}  // namespace flux::modules
