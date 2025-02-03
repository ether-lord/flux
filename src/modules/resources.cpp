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
