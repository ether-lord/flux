#version 460 core
struct VertexData {
   float position[3];
   float normal[3];
   float uv[2];
   float texture_index;
};

layout(binding = 0, std430) readonly buffer ssbo1 {
   VertexData data[];
};

vec3 getPosition(int index) {
   return vec3 (
      data[index].position[0], 
      data[index].position[1], 
      data[index].position[2]
   );
}

vec2 getUV(int index) {
    return vec2 (
      data[index].uv[0], 
      data[index].uv[1]
   );
}

vec3 getNormal(int index) {
   return vec3 (
      data[index].normal[0], 
      data[index].normal[1], 
      data[index].normal[2]
   );
}

uniform mat4 projection;
uniform mat4 view;

out vec2 a_uv;
out vec3 a_normal;
flat out float a_tex_index;

void main()
{
   gl_Position = projection * view * vec4(getPosition(gl_VertexID), 1.0);

   a_uv = getUV(gl_VertexID);
   a_normal = getNormal(gl_VertexID);
   a_tex_index = data[gl_VertexID].texture_index;
}