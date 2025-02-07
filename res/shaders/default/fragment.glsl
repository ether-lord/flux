#version 460 core
out vec4 FragColor;
  
in vec2 a_uv;
in vec3 a_normal;
flat in float a_tex_index;

uniform sampler2D u_textures[4];

void main()
{
   FragColor = texture(u_textures[int(a_tex_index)], a_uv);
}