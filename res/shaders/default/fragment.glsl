#version 460 core
out vec4 FragColor;
  
in vec2 a_uv;
in vec3 a_normal;
flat in float a_tex_index;

uniform sampler2D u_textures[4];

void main()
{
   vec3 light_position = vec3(-1.0, 0.75, 0.4);
   vec3 directional_light = vec3(1.0);
   float light_strength = 0.1;
   float directional_magnitude = max(0, dot(light_position, a_normal));
   directional_light *= directional_magnitude * light_strength;

   FragColor = vec4(vec3(0.5) + directional_light,1.0);
}