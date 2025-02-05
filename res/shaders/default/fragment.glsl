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
uniform vec3 u_view_position;

void main()
{
    vec3 ambient = u_ambient_color * u_ambient_intensity;

    vec3 normal = normalize(a_normal);
    vec3 light_dir = normalize(u_light_position - a_frag_position);  

    float diff = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = diff * vec3(1.0);

    vec3 view_direction = normalize(u_view_position - a_frag_position);
    vec3 reflection_direction = reflect(-light_dir, normal);

    float spec = pow(max(dot(view_direction, reflection_direction), 0.0), 32);
    vec3 specular = 0.5 * spec * vec3(1.0);

    FragColor = texture(u_textures[texIndex], texCoord) * vec4(ambient + diffuse + specular, 1.0);
}