#version 330 core
struct Material {
  sampler2D diffuse1;
  sampler2D diffuse2;
  sampler2D diffuse3;
  sampler2D specular1;
  sampler2D specular2;
  float shiny;
};

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float constant;
  float linear;
  float quadratic;
};

in vec2 texture_coord;
in vec3 vertex_normal; 
in vec3 frag_pos;
in vec3 view_pos;

#define MAX_LIGHTS 10
uniform int light_count;
uniform Light lights[MAX_LIGHTS];
uniform Material material;

out vec4 FragColor;


vec3 get_ambient(Light _light, Material material, vec4 ambient_texture) {
    return vec3(_light.ambient * ambient_texture.rgb);
}

vec3 get_diffuse(Light _light, vec3 norm, Material material, vec4 diffuse_texture) {
    vec3 light_dir = normalize(_light.position - frag_pos);
    float diff = max(dot(norm, light_dir), 0.f);
    return vec3(_light.diffuse * diff * diffuse_texture.rgb);
}

vec3 get_specular(Light _light, vec3 norm, Material material, vec4 specular_texture) {

    vec3 light_dir = normalize(_light.position - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    vec3 view_dir = normalize(view_pos - frag_pos);
    float reflectance = max(dot(view_dir, reflect_dir), 0.001f);
    float spec = pow(reflectance, material.shiny);
    return vec3(_light.specular * spec * specular_texture.rgb);
}

vec3 get_pointlight(Light _light, vec3 norm, Material material, vec4 ambient_texture, vec4 diffuse_texture, vec4 specular_texture) {
    vec3 ambient  = get_ambient(_light, material, ambient_texture);
    vec3 diffuse  = get_diffuse(_light, norm, material, diffuse_texture);
    vec3 specular = get_specular(_light, norm, material, specular_texture);

    float dist = length(_light.position - frag_pos);
    float attenuation = 1.f / (_light.constant + dist * _light.linear + dist*dist*_light.quadratic);

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return ambient + diffuse;
}


void main() {

    vec3 norm = normalize(vertex_normal);
    vec4 ambient_texture = texture(material.diffuse1, texture_coord);
    vec4 diffuse_texture = texture(material.diffuse2, texture_coord);
    vec4 specular_texture = texture(material.specular1, texture_coord);
    if (ambient_texture.a < 0.05)
        discard;

    vec3 color = vec3(0.f);
    for (int i = 0; i < light_count; i++)
        color += get_pointlight(lights[i], norm, material, ambient_texture, diffuse_texture, specular_texture);

    FragColor = vec4(color, 1.f);
}


