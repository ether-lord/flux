#include "resources.h"

#define STB_IMAGE_IMPLEMENTATION

#include <glad.h>

#include <functional>
#include <iostream>

#include "components/graphics.h"
#include "components/shader.h"
#include "resources_manager.h"
#include "stb_image.h"

using namespace std;
using namespace flux::components;
using namespace flux::resources;

namespace flux::modules {

ShaderLoader::ShaderLoader(flecs::world& world) {
 
}

TextureLoader::TextureLoader(flecs::world& world) {
  world.component<Texture>();
  world.component<TextureBuffer>();

  stbi_set_flip_vertically_on_load(true);

  world.system<const Texture>("Texture Loader")
      .kind(flecs::OnLoad)
      .each([](flecs::entity e, const Texture& texture_data) {
        auto texture_path =
            ResourcesManager::get().GetPathToTexture(texture_data.name);

        int width, height, nr_channels;
        unsigned char* data =
            stbi_load(texture_path.c_str(), &width, &height, &nr_channels, 0);

        unsigned int texture;
        glGenTextures(1, &texture);

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        if (data) {
          glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                       GL_UNSIGNED_BYTE, data);
          glGenerateMipmap(GL_TEXTURE_2D);
        } else {
          // Handle errors
        }
        stbi_image_free(data);

        e.remove<Texture>();
        e.set<TextureBuffer>({texture});
      });
}

Buffering::Buffering(flecs::world& world) {
  world.component<Mesh>();
  world.component<MeshBuffer>();

  world.system<const Mesh>("Mesh Buffering System")
      .kind(flecs::OnLoad)
      .each([](flecs::entity e, const Mesh& mesh_data) {
        const auto& vertices = mesh_data.vertices;
        const auto& indices = mesh_data.indices;

        vector<float> vbo_data;
        for (const auto& v : vertices) {
          const auto& pos = v.position;
          const auto& uv = v.uv;
          vbo_data.push_back(pos.x);
          vbo_data.push_back(pos.y);
          vbo_data.push_back(pos.z);
          vbo_data.push_back(uv.x);
          vbo_data.push_back(uv.y);
        }

        unsigned int vao, vbo, ebo;

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vbo_data[0]) * vbo_data.size(),
                     &vbo_data[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(indices[0]) * indices.size(), &indices[0],
                     GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void*)offsetof(Vertex, uv));
        glEnableVertexAttribArray(1);

        e.remove<Mesh>();
        e.set<MeshBuffer>(
            MeshBuffer{.vao = vao, .indices = (unsigned int)indices.size()});
      });
}

}  // namespace flux::modules
