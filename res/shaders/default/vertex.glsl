#version 460 core

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_uv;
layout(location = 3) in int v_texture;

uniform mat4 projection;
uniform mat4 view;

out vec2 a_uv;
out vec3 a_normal;
flat out float a_tex_index;

void main()
{
   gl_Position = projection * view * vec4(v_position, 1.0);

   a_uv = v_uv;
   a_normal = v_normal;
   a_tex_index = v_texture;
}