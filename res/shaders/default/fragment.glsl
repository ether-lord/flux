#version 460 core
out vec4 FragColor;
  
in vec2 texCoord;
in vec3 a_normal;
in vec3 a_frag_position;
flat in int texIndex;

uniform sampler2D u_textures[4];

uniform float u_ambient_intensity;
uniform vec3 u_ambient_color;
uniform vec3 u_light_position;

void main()
{
    vec3 ambient = u_ambient_color * u_ambient_intensity;

    vec3 norm = normalize(a_normal);
    vec3 light_dir = normalize(u_light_position - a_frag_position);  

    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * vec3(1.0);

    FragColor = texture(u_textures[texIndex], texCoord) * vec4(ambient + diffuse, 1.0);
}