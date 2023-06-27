#include "application.hpp"
#include "glm/ext/vector_float3.hpp"
#include "render/asset/model_loader.hpp"
#include "render/asset/texture_loader.hpp"
#include "render/camera/camera.hpp"
#include "render/opengl/device.hpp"
#include "render/renderer/forward_pass.hpp"
#include "render/scene/scene_model.hpp"
#include "render/scene/transform.hpp"
#include "util/timer.hpp"
#include <memory>
#include <type_traits>

namespace cppsim {

Application::Application()
{
    Log::init();
    Device::init();
    window = new Window(WindowWidth, WindowHeight);
    Device::set_current_window(*window);
}


void Application::init_scene()
{
    const char* cube_shader_path = "/Users/philiptoftenielsen/dev/cppsim/src/render/shaders/color.shader";
    const char* plane_shader_path = "/Users/philiptoftenielsen/dev/cppsim/src/render/shaders/texture.shader";

    auto cube_shader = std::make_shared<Shader>(cube_shader_path);
    auto plane_shader = std::make_shared<Shader>(plane_shader_path);

    ModelLoader loader("/Users/philiptoftenielsen/dev/cppsim/model/plane.obj");
    auto plane_mesh = loader.to_mesh();
    plane_mesh.set_shader(plane_shader);
    TextureLoader tex_loader("/Users/philiptoftenielsen/dev/cppsim/texture/stone.jpg");
    Texture plane_tex = tex_loader.create_texture();
    plane_mesh.set_texture(plane_tex);
    DrawCall plane_draw_call(0, static_cast<i32>(plane_mesh.get_num_vertices()));
    plane_mesh.set_draw_call(plane_draw_call);
    auto plane_trans = std::make_shared<Transform>();
    plane_trans->set_translation(glm::vec3(-25.0f, 0.0f, -25.0f));
    auto plane_model = std::make_shared<SceneModel>("plane", plane_trans, plane_mesh);


    ModelLoader cube_loader("/Users/philiptoftenielsen/dev/cppsim/model/monkey.obj");
    auto cube_mesh = cube_loader.to_mesh();
    cube_mesh.set_shader(cube_shader);
    DrawCall cube_draw_call(0, static_cast<i32>(cube_mesh.get_num_vertices()), DrawCall::PolygonMode::Lines);
    cube_mesh.set_draw_call(cube_draw_call);
    auto trans_ptr = std::make_shared<Transform>();
    trans_ptr->set_scale(glm::vec3(0.2f));
    trans_ptr->set_translation(glm::vec3(0.0f, 0.2f, 0.0f));
    auto cube_model = std::make_shared<SceneModel>("cube", trans_ptr, cube_mesh);


    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    camera->set_view_matrix(glm::vec3(-1.0f, 0.5f, -2.0f), glm::vec3(0));
    camera->set_perspective_projection(glm::radians(45.0f), window->aspect_ratio(), 0.1f, 100.0f);
    std::shared_ptr<SceneCamera> scene_camera = std::make_shared<SceneCamera>("cam 1", camera);
    cam_controller = CameraController(scene_camera);

    scene = std::make_unique<Scene>(scene_camera);
    scene->add_node(plane_model);
    scene->add_node(cube_model);
}

void Application::init_renderer()
{
    renderer = make_unique<Renderer>(scene->get_scene_graph(), scene->get_scene_camera()->get_camera());
    renderer->add_render_pass(std::make_unique<ForwardPass>());

}

void Application::run()
{
    window->set_mouse_visible(false);
    Device::enable_feature(GL_DEPTH_TEST);
    Device::set_vsync(true);


    init_scene();
    init_renderer();


    loop();


    window->terminate();
    Device::terminate();
}

void Application::render(float /*dt*/)
{
    Device::clear(true, glm::vec3(0.1f, 0.1f, 0.1f), true, false, 0);
    renderer->render();

}

void Application::loop()
{
    Timer frame_timer;
    float dt = 0.01667f;

    while (!window->should_close())
    {
        frame_timer.start();
        cam_controller.update(*window, dt);

        render(dt);
        CPPSIM_TRACE(dt);

        window->swap_buffers();
        Device::poll_events();
        Device::process_errors();

        frame_timer.stop();
        dt = frame_timer.elapsed<Nano>() / 1000000000.0f;
    }
}

} // namespace cppsim