#include "resources.h"

#include <glad.h>

#include <iostream>

#include "components/graphics.h"
#include "components/shader.h"
#include "resources_manager.h"

using namespace std;
using namespace flux::components;
using namespace flux::resources;

namespace flux::modules {

Buffer2d::Buffer2d(flecs::world& world) {
  world.component<Shape2d>();

  world.system<Shape2d>()
      .kind(flecs::OnLoad)
      .each([](flecs::entity e, Shape2d& shape) {
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

        e.remove<Shape2d>();
        e.set<Shape2dBuffered>(Shape2dBuffered{vao, indices.size()});
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

}  // namespace flux::modules
