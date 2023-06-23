#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "core/log.hpp"
#include "core/core.hpp"

#include <iostream>

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include "render/asset/model_loader.hpp"
#include "render/asset/texture_loader.hpp"
#include "render/opengl/draw_call.hpp"
#include "render/opengl/vertex_buffer.hpp"
#include "render/opengl/vertex_buffer_layout.hpp"
#include "render/opengl/vertex_array.hpp"
#include "render/opengl/index_buffer.hpp"
#include "render/opengl/gl_shader.hpp"
#include "render/opengl/texture.hpp"
#include "util/timer.hpp"
#include "render/window.hpp"

#include "render/camera/camera.hpp"
#include "render/camera/camera_controller.hpp"
#include "render/scene/scene_camera.hpp"
#include "render/scene/transform.hpp"

#include "util/timer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

using namespace cppsim;


void poll_input(const Window& window, float& rot_x, float& rot_y)
{
    if (window.is_key_pressed(GLFW_KEY_J))
        rot_y += 0.01f;
    else if (window.is_key_pressed(GLFW_KEY_L))
        rot_y -= 0.01f;
    if (window.is_key_pressed(GLFW_KEY_I))
        rot_x += 0.01f;
    else if (window.is_key_pressed(GLFW_KEY_K))
        rot_x -= 0.01f;
}

int main(int /*argc*/, char** /*argv*/)
{
    Log::init();
    CPPSIM_CORE_ERROR("Hello, World!");

    Window window;
    window.set_mouse_visible(false);

    Shader cube_shader("/Users/philiptoftenielsen/dev/cppsim/src/render/shaders/color.shader");
    cube_shader.bind();

    Shader plane_shader("/Users/philiptoftenielsen/dev/cppsim/src/render/shaders/texture.shader");


    float cube_verts[] = { // front
        -1.0,
        -1.0,
        1.0,
        1.0,
        -1.0,
        1.0,
        1.0,
        1.0,
        1.0,
        -1.0,
        1.0,
        1.0,
        // back
        -1.0,
        -1.0,
        -1.0,
        1.0,
        -1.0,
        -1.0,
        1.0,
        1.0,
        -1.0,
        -1.0,
        1.0,
        -1.0
    };

    u16 cube_indices[] = { // front
        0,
        1,
        2,
        2,
        3,
        0,
        // right
        1,
        5,
        6,
        6,
        2,
        1,
        // back
        7,
        6,
        5,
        5,
        4,
        7,
        // left
        4,
        0,
        3,
        3,
        7,
        4,
        // bottom
        4,
        5,
        1,
        1,
        0,
        4,
        // top
        3,
        2,
        6,
        6,
        7,
        3
    };

    ModelLoader loader("/Users/philiptoftenielsen/dev/cppsim/model/plane.obj");
    auto plane_mesh = loader.to_mesh();
    plane_mesh.set_shader(plane_shader);

    TextureLoader tex_loader("/Users/philiptoftenielsen/dev/cppsim/texture/stone.jpg");
    Texture plane_tex = tex_loader.create_texture();
    plane_mesh.set_texture(plane_tex);

    DrawCall plane_draw_call(0, static_cast<i32>(plane_mesh.get_num_vertices()));
    plane_mesh.set_draw_call(plane_draw_call);


    VertexBuffer vbo(&cube_verts[0], 3, 8 * sizeof(float));

    VertexBufferLayout vbo_layout;
    vbo_layout.push<float>(3);

    VertexArray vao_cube;
    vao_cube.add_buffer(vbo, vbo_layout);
    vao_cube.bind();

    IndexBuffer ib(&cube_indices[0], 36);


    glEnable(GL_DEPTH_TEST);


    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    camera->set_view_matrix(glm::vec3(-1.0f, 0.5f, -2.0f), glm::vec3(0));
    camera->set_perspective_projection(glm::radians(45.0f), window.aspect_ratio(), 0.1f, 100.0f);

    std::shared_ptr<SceneCamera> scene_camera = std::make_shared<SceneCamera>("cam 1", camera);


    CPPSIM_TRACE("{0}", window.aspect_ratio());


    CameraController cam_controller(scene_camera);

    float rot_x = 0;
    float rot_y = 0;

    Timer frame_timer;
    float dt = 0.01667f;

    while (!window.should_close())
    {
        frame_timer.start();
        cam_controller.update(window, dt);
        poll_input(window, rot_x, rot_y);


        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 s(0.2f);
        glm::vec3 rot(rot_x, rot_y, 0.0f);
        auto transform = Transform();

        transform.set_scale(s);
        transform.set_translation(glm::vec3(0.0, 0.20f, 0.0f));
        // transform.set_rotation(rot);

        auto trans = camera->project_view_matrix() * transform.transform_matrix();


        cube_shader.bind();
        cube_shader.set_uniform_mat4f("model", trans);
        vao_cube.bind();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);


        transform.set_translation(glm::vec3(-5.0f, 0.0f, -5.0));
        trans = camera->project_view_matrix() * transform.transform_matrix();
        plane_mesh.get_shader().bind();
        plane_mesh.get_shader().set_uniform_mat4f("model", trans);

        plane_mesh.draw();


        window.swap_buffers();
        window.poll_events();

        GLenum err = 0;
        while ((err = glGetError()) != GL_NO_ERROR)
        {
            CPPSIM_TRACE(err);
        }


        frame_timer.stop();

        dt = frame_timer.elapsed<Nano>() / 1000000000.0f;
    }


    glfwTerminate();
    return 0;
}