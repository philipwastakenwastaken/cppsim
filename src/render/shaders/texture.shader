#shader vertex
#version 330 core
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_tex_coord;
layout(location = 2) in vec3 a_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 light_pos;
uniform vec3 light_color;

out vec2 tex_coord;

void main() {
    gl_Position = proj * view * model * vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);

    tex_coord = a_tex_coord;
}

#shader fragment
#version 330 core
in vec2 tex_coord;
out vec4 frag_color;

uniform sampler2D sampler;

void main() {
    frag_color = texture(sampler, tex_coord);
}
