#shader vertex
#version 330 core
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_tex_coord;
layout(location = 2) in vec3 a_normal;
uniform mat4 model;

out vec3 color;

void main() {
    gl_Position = model * vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);

    color = (a_pos + 1) / 2;
}

#shader fragment
#version 330 core
in vec3 color;
out vec4 frag_color;

void main() {
    frag_color = vec4(color.x, color.y, color.z, 1.0f);
}
