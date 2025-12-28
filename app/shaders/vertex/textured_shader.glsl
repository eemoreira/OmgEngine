#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex;

out vec2 texture_coord;
out vec3 vertex_normal; 
out vec3 frag_pos;
out vec3 view_pos;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normal_matrix;
uniform vec3 camera_pos;

void main()
{
    gl_Position = projection * view * model * vec4(a_pos, 1.f);
    texture_coord = a_tex;
    vertex_normal = normalize(normal_matrix * a_normal);
    frag_pos = vec3(model * vec4(a_pos, 1.f));
    view_pos = camera_pos;
}


