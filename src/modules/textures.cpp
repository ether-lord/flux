#include "textures.h"

#define STB_IMAGE_IMPLEMENTATION

#include <glad.h>
#include <stb_image.h>

#include <iostream>

#include "resources_manager.h"

using namespace flux::resources;

namespace flux {

Textures::Textures(flecs::world& world) {
  world.component<DiffuseMap>();
  world.component<LoadedTextures>();

  world.add<LoadedTextures>();

  stbi_set_flip_vertically_on_load(true);

  world.observer<DiffuseMap>("Texture Loader")
      .event(flecs::OnSet)
      .each([](flecs::entity e, DiffuseMap& texture) {
        auto& texture_path_to_id =
            e.world().get_mut<LoadedTextures>()->texture_path_to_id;
        auto texture_path =
            ResourcesManager::get().GetAbsolutePath(texture.path);

        if (texture_path_to_id.count(texture_path)) {
          texture.id = texture_path_to_id[texture_path];
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
          auto image_format = nr_channels == 4 ? GL_RGBA : GL_RGB;
          glTexImage2D(GL_TEXTURE_2D, 0, image_format, width, height, 0,
                       image_format, GL_UNSIGNED_BYTE, data);
          glGenerateMipmap(GL_TEXTURE_2D);
        } else {
          flecs::log::err("failed to loade the texture from: %s",
                          texture_path.c_str());
        }
        stbi_image_free(data);

        texture.id = texture_id;
        texture_path_to_id[texture_path] = texture_id;
      });
}

}  // namespace flux