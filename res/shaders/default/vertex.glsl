#version 460 core
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in int aTexIndex;

out vec2 texCoord;
out vec3 a_normal;
out vec3 a_frag_position;
flat out int texIndex;

uniform mat4 view;
uniform mat4 projection;

void main()
{
   gl_Position = projection * view * vec4(v_position, 1.0f);
   texCoord = aTexCoord;
   texIndex = aTexIndex;
   a_normal = v_normal;
   a_frag_position = v_position;
} 