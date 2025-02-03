#include "textures.h"

#define STB_IMAGE_IMPLEMENTATION

#include <glad.h>

#include "resources_manager.h"
#include "stb_image.h"

using namespace flux::resources;

namespace flux {

Textures::Textures(flecs::world& world) {
  world.component<TextureData>();
  world.component<Texture>();

  stbi_set_flip_vertically_on_load(true);

  world.system<const TextureData>("Texture Loader")
      .kind(flecs::OnLoad)
      .each([](flecs::entity e, const TextureData& texture_data) {
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

        e.remove<TextureData>();
        e.set<Texture>({texture});
      });
}

}  // namespace flux