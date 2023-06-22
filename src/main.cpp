#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "core/log.hpp"
#include "core/core.hpp"

#include <iostream>

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
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

    Shader shader("/Users/philiptoftenielsen/dev/cppsim/src/render/shaders/color.shader");
    shader.bind();


    float cube_verts[] = {
        // front
        -1.0, -1.0,  1.0,
        1.0, -1.0,  1.0,
        1.0,  1.0,  1.0,
        -1.0,  1.0,  1.0,
        // back
        -1.0, -1.0, -1.0,
        1.0, -1.0, -1.0,
        1.0,  1.0, -1.0,
        -1.0,  1.0, -1.0
    };

    u32 cube_indices[] = {
	    // front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};


    Texture

    VertexBuffer vbo(&cube_verts[0], 3, 8 * sizeof(float));

    VertexBufferLayout vbo_layout;
    vbo_layout.push<float>(3);

    VertexArray vao_cube;
    vao_cube.add_buffer(vbo, vbo_layout);
    vao_cube.bind();

    IndexBuffer ib(&cube_indices[0], 36);


    float plane_verts[] = {
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    u32 plane_indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    VertexBuffer vbo_plane(&plane_verts[0], 4, 3 * sizeof(float));
    VertexBufferLayout plane_layout;
    plane_layout.push<float>(3);

    VertexArray vao_plane;
    vao_plane.add_buffer(vbo_plane, plane_layout);

    IndexBuffer ib_plane(&plane_indices[0], 6);



    glEnable(GL_DEPTH_TEST);


    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    camera->set_view_matrix(glm::vec3(-1.0f, 0.0f, -2.0f), glm::vec3(0));
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
        //transform.set_rotation(rot);

        auto trans = camera->project_view_matrix() * transform.transform_matrix();

        shader.set_uniform_mat4f("model", trans);

        vao_cube.bind();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

        transform.set_translation(glm::vec3(-25.0f, 0.0f, -25.0f));
        transform.set_rotation(glm::vec3(glm::radians(90.0f), 0.0f, 0.0f));
        transform.set_scale(glm::vec3(50.0f));
        trans = camera->project_view_matrix() * transform.transform_matrix();
        shader.set_uniform_mat4f("model", trans);

        vao_plane.bind();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        window.swap_buffers();
        window.poll_events();

        frame_timer.stop();

        dt = frame_timer.elapsed<Nano>() / 1000000000.0f;
    }


    glfwTerminate();
    return 0;
}