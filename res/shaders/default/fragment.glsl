#version 460 core
out vec4 FragColor;
  
in vec2 texCoord;
flat in int texIndex;

uniform sampler2D u_textures[4];

void main()
{
    FragColor = texture(u_textures[texIndex], texCoord);
}