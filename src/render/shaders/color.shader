#shader vertex
#version 330 core
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_tex_coord;
layout(location = 2) in vec3 a_normal;

uniform vec3 light_pos;
uniform vec3 light_color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 frag_pos;
out vec3 frag_normal;
out vec3 frag_light_pos;
out vec3 frag_light_color;

void main() {
    gl_Position = proj * view * model * vec4(a_pos, 1.0);
    frag_pos = vec3(view * model * vec4(a_pos, 1.0));
    frag_normal = mat3(transpose(inverse(view * model))) * a_normal;
    frag_light_pos = vec3(view * vec4(light_pos, 1.0));
    frag_light_color = light_color;
}

#shader fragment
#version 330 core

in vec3 frag_pos;
in vec3 frag_light_pos;
in vec3 frag_normal;
in vec3 frag_light_color;

out vec4 frag_color;

void main() {
    vec3 object_color = vec3(0.7);

    float ambient_strength = 0.1;
    vec3 ambient = ambient_strength * frag_light_color;

    vec3 norm = normalize(frag_normal);
    vec3 light_dir = normalize(frag_light_pos - frag_pos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * frag_light_color;

    float specular_strength = 0.5;
    vec3 view_dir = normalize(-frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
    vec3 specular = specular_strength * spec * frag_light_color;

    vec3 result = (0 + diffuse + specular) * object_color;


    frag_color = vec4(result, 1.0f);
}
