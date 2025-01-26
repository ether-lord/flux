#include "render.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "entity_manager.h"
#include "glad.h"
#include "resources_manager.h"
#include "shader.h"
#include "shader_program.h"

using namespace std;
using namespace flux::shader;

using flux::resources::ResourcesManager;

namespace flux::systems {

void Render::Update() { RenderDrawable2d(); }

void Render::RenderDrawable2d() {
  auto drawables = entity::EntityManager::get().GetDrawables2d();

  for (const auto& dr : drawables) {
    auto position = dr.position;
    auto polygon = dr.polygon;

    auto indices = polygon->indices;
    auto colors = polygon->colors;
    auto vertices = polygon->vertices;

    vector<float> vbo_data;

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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(),
                 &indices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,
                          (void*)(sizeof(vertices[0]) * vertices.size()));
    glEnableVertexAttribArray(1);

    auto shader_src = ResourcesManager::get().GetShaderSource("basic_vertex");
    auto vertex_shader = Shader(ShaderType::VERTEX, shader_src);
    vertex_shader.Compile();
    if (!vertex_shader.GetCompileStatus()) {
      cout << vertex_shader.GetInfoLog() << endl;
    }

    shader_src = ResourcesManager::get().GetShaderSource("basic_frag");
    auto frag_shader = Shader(ShaderType::FRAGMENT, shader_src);
    frag_shader.Compile();
    if (!frag_shader.GetCompileStatus()) {
      cout << frag_shader.GetInfoLog() << endl;
    }

    auto shader_program = ShaderProgram();
    shader_program.AttachShader(vertex_shader);
    shader_program.AttachShader(frag_shader);
    shader_program.Link();
    shader_program.Use();

    if (!shader_program.GetLinkStatus()) {
      cout << shader_program.GetInfoLog() << endl;
    }

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    shader_program.Delete();
  }
}

}  // namespace flux::systems
