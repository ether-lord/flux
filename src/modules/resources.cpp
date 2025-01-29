#include "resources.h"

#include <glad.h>

#include <functional>
#include <iostream>

#include "components/graphics.h"
#include "components/shader.h"
#include "resources_manager.h"

using namespace std;
using namespace flux::components;
using namespace flux::resources;

namespace flux::modules {

Buffer2d::Buffer2d(flecs::world& world) {
  world.component<Shape2dData>();

  world.system<Shape2dData>()
      .kind(flecs::OnLoad)
      .each([](flecs::entity e, Shape2dData& shape) {
        auto indices = shape.indices;
        auto colors = shape.colors;
        auto vertices = shape.vertices;

        std::vector<float> vbo_data;
        for (const auto& i : vertices) {
          vbo_data.push_back(i.x);
          vbo_data.push_back(i.y);
          vbo_data.push_back(i.z);
        }
        for (const auto& i : colors) {
          vbo_data.push_back(i.x);
          vbo_data.push_back(i.y);
          vbo_data.push_back(i.z);
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,
                              (void*)(sizeof(vertices[0]) * vertices.size()));
        glEnableVertexAttribArray(1);

        e.remove<Shape2dData>();
        e.set<Shape2d>(Shape2d{vao, indices.size()});
      });
}

ShaderLoader::ShaderLoader(flecs::world& world) {
  world.component<ShaderData>();
  world.component<Shader>();

  world.system<Shader>("Shader program initializer")
      .kind(flecs::OnLoad)
      .each([](Shader& shader) { shader.id = glCreateProgram(); });

  world.system<Shader, const ShaderData>("Shader data loader")
      .kind(flecs::OnLoad)
      .each([](flecs::entity e, Shader& shader_program,
               const ShaderData& shader_data) {
        for (const auto& shader : shader_data.data) {
          auto shader_source =
              ResourcesManager::get().GetShaderSource(shader.name);
          auto shader_source_cstr = shader_source.c_str();
          auto shader_id = glCreateShader(shader.type);
          glShaderSource(shader_id, 1, &shader_source_cstr, NULL);

          int compile_status = 0;
          glCompileShader(shader_id);
          glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status);

          if (!compile_status) {
            std::string log;
            log.reserve(LOG_BUFF_SIZE);

            glGetShaderInfoLog(shader_id, LOG_BUFF_SIZE, NULL, log.data());
            cout << log << endl;
          }

          glAttachShader(shader_program.id, shader_id);
        }

        e.remove<ShaderData>();
      });

  world.system<Shader>("Shader linker")
      .kind(flecs::OnLoad)
      .each([](const Shader& shader) { glLinkProgram(shader.id); });
}

ModelLoader::ModelLoader(flecs::world& world) {
  world.component<Mesh>();
  world.component<MeshData>();
  world.component<Model>();

  world.system<Model>().kind(flecs::OnLoad).each([=](Model& model) {
    auto model_path = ResourcesManager::get().GetAbsolutePath(model.path);
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        model_path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
      // Handle errors;
    }

    std::function<void(aiNode * node, const aiScene* scene)> process_mesh =
        [&](aiNode* node, const aiScene* scene) {

        };

    std::function<void(aiNode * node, const aiScene* scene)> process_node =
        [&](aiNode* node, const aiScene* scene) {
          for (auto i = 0; i < node->mNumMeshes; ++i) {
            auto mesh = scene->mMeshes[node->mMeshes[i]];

            vector<Vertex> vertices;
            vector<unsigned int> indices;
            vector<Texture> Textures;

            for (auto i = 0; i < mesh->mNumVertices; ++i) {
              Vertex vertex;

              glm::vec3 pos;
              pos.x = mesh->mVertices[i].x;
              pos.y = mesh->mVertices[i].y;
              pos.z = mesh->mVertices[i].z;
              vertex.position = pos;

              glm::vec3 normals;
              normals.x = mesh->mNormals[i].x;
              normals.y = mesh->mNormals[i].y;
              normals.z = mesh->mNormals[i].z;
              vertex.normal = normals;

              if (mesh->mTextureCoords[0]) {
                glm::vec2 uv;
                uv.x = mesh->mTextureCoords[0][i].x;
                uv.y = mesh->mTextureCoords[0][i].y;
                vertex.uv = uv;
              } else {
                vertex.uv = glm::vec2(0.0f, 0.0f);
              }

              vertices.push_back(vertex);
            }
          }

          for (auto i = 0; i < node->mNumChildren; ++i) {
            process_node(node->mChildren[i], scene);
          }
        };

    process_node(scene->mRootNode, scene);
  });
}

}  // namespace flux::modules
