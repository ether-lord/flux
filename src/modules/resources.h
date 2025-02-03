#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <flecs.h>

#include <assimp/Importer.hpp>


#define LOG_BUFF_SIZE 1024u

struct Buffering {
  Buffering(flecs::world& world);
};

