#pragma once

#include <flecs.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace flux::modules {

#define LOG_BUFF_SIZE 1024u

struct Buffer2d {
  Buffer2d(flecs::world& world);
};

struct ShaderLoader {
  ShaderLoader(flecs::world& world);
};

struct ModelLoader {
  ModelLoader(flecs::world& world);
};

}  // namespace flux::modules
