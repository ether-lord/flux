#include "textures.h"

#define STB_IMAGE_IMPLEMENTATION

#include <glad.h>
#include <stb_image.h>

#include "resources_manager.h"

using namespace flux::resources;

namespace flux {

Textures::Textures(flecs::world& world) {
  world.component<Texture>();
  world.component<LoadedTextures>();

  world.add<LoadedTextures>();

  stbi_set_flip_vertically_on_load(true);

  world.observer<Texture>("Texture Loader")
      .event(flecs::OnSet)
      .each([](flecs::entity e, Texture& texture) {
        auto loaded_textures = e.world().get_mut<LoadedTextures>();
        auto texture_path =
            ResourcesManager::get().GetPathToTexture(texture.name);

        if (loaded_textures->texture_path_to_id.count(texture_path)) {
          texture.id = loaded_textures->texture_path_to_id[texture_path];
          return;
        }

        int width, height, nr_channels;
        unsigned char* data =
            stbi_load(texture_path.c_str(), &width, &height, &nr_channels, 0);

        unsigned int texture_id;
        glGenTextures(1, &texture_id);

        glBindTexture(GL_TEXTURE_2D, texture_id);
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

        texture.id  = texture_id;
        loaded_textures->texture_path_to_id[texture_path] = texture_id;
      });
}

}  // namespace flux