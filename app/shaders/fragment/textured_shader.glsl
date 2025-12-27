#version 330 core
struct Material {
  sampler2D diffuse1;
  sampler2D diffuse2;
  sampler2D diffuse3;
  sampler2D specular1;
  sampler2D specular2;
  float shiny;
};

in vec2 texture_coord;

uniform Material material;

out vec4 FragColor;

void main() {

    vec4 ambient_texture = mix(texture(material.diffuse1, texture_coord), texture(material.diffuse2, texture_coord), 0.5);
    ambient_texture = mix(ambient_texture, texture(material.diffuse3, texture_coord), 0.5);
    if (ambient_texture.a < 0.05)
        discard;

    FragColor = vec4(ambient_texture);
}


