#version 330 core

out vec4 FragColor;

void main() {
  vec3 color = vec3(0.f, 0.f, 1.f);
  FragColor = vec4(color, 1.f);
}

