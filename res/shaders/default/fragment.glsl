#version 460 core
out vec4 FragColor;
  
in vec2 texCoord;
flat in int texIndex;

uniform sampler2D u_textures[4];

uniform float u_ambient_intensity;
uniform vec3 u_ambient_color;

void main()
{
    vec3 ambient = u_ambient_color * u_ambient_intensity;
    FragColor = texture(u_textures[texIndex], texCoord) * vec4(ambient, 1.0);
}