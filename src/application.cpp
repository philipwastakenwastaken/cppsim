#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "application.hpp"
#include "glm/ext/vector_float3.hpp"
#include "render/asset/model_loader.hpp"
#include "render/asset/texture_loader.hpp"
#include "render/camera/camera.hpp"
#include "render/lightning/point_light.hpp"
#include "render/opengl/device.hpp"
#include "render/renderer/forward_pass.hpp"
#include "render/scene/scene_model.hpp"
#include "render/scene/transform.hpp"
#include "render/shaders/shader_updates.hpp"

#include "util/frame_timer.hpp"

#include <algorithm>
#include <memory>
#include <type_traits>
#include <functional>

namespace cppsim {

Application::Application()
{
    Log::init();
    Device::init();
    window = new Window(WindowWidth, WindowHeight);
    Device::set_current_window(*window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window->get_window(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
}


void Application::init_scene()
{

    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    camera->set_view_matrix(glm::vec3(-1.0f, 0.5f, -2.0f), glm::vec3(0));
    camera->set_perspective_projection(glm::radians(45.0f), window->aspect_ratio(), 0.1f, 100.0f);
    std::shared_ptr<SceneCamera> scene_camera = std::make_shared<SceneCamera>("cam 1", camera);
    cam_controller = CameraController(scene_camera);
    scene = std::make_unique<Scene>(scene_camera);

    renderer = make_unique<Renderer>(scene->get_scene_graph(), scene->get_scene_camera()->get_camera());

    const char* cube_shader_path = "/Users/philiptoftenielsen/dev/cppsim/src/render/shaders/color.shader";
    const char* plane_shader_path = "/Users/philiptoftenielsen/dev/cppsim/src/render/shaders/texture.shader";

    auto cube_shader = std::make_shared<Shader>(cube_shader_path);
    auto plane_shader = std::make_shared<Shader>(plane_shader_path);


    UpdateTransformFunction f1 = std::bind(projection_model_view_transform, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    renderer->add_update_transform_function(cube_shader, f1);
    renderer->add_update_transform_function(plane_shader, f1);

    UpdateLightFunction f2 = std::bind(point_light_update, std::placeholders::_1, std::placeholders::_2);
    renderer->add_update_light_function(cube_shader, f2);


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


    ModelLoader monkey_loader("/Users/philiptoftenielsen/dev/cppsim/model/suzanne.obj");
    auto monkey_mesh = monkey_loader.to_mesh();
    monkey_mesh.set_shader(cube_shader);
    DrawCall monkey_draw_call(0, static_cast<i32>(monkey_mesh.get_num_vertices()), DrawCall::PolygonMode::Fill);
    monkey_mesh.set_draw_call(monkey_draw_call);
    auto monkey_trans = std::make_shared<Transform>();
    monkey_trans->set_scale(glm::vec3(0.2f));
    monkey_trans->set_translation(glm::vec3(0.0f, 0.2f, 0.0f));
    auto monkey_model = std::make_shared<SceneModel>("monkey", monkey_trans, monkey_mesh);
    scene->add_node(monkey_model);

    ModelLoader cube_loader("/Users/philiptoftenielsen/dev/cppsim/model/cube.obj");
    auto cube_mesh = cube_loader.to_mesh();
    cube_mesh.set_shader(plane_shader);
    cube_mesh.set_texture(plane_tex);
    DrawCall cube_draw_call(0, static_cast<i32>(cube_mesh.get_num_vertices()), DrawCall::PolygonMode::Fill);
    cube_mesh.set_draw_call(cube_draw_call);
    auto trans_ptr = std::make_shared<Transform>();
    trans_ptr->set_scale(glm::vec3(0.1f));
    trans_ptr->set_translation(glm::vec3(1.0f));
    auto cube_model = std::make_shared<SceneModel>("cube", trans_ptr, cube_mesh);
    scene->add_node(cube_model);


    scene->add_node(plane_model);

}

void Application::init_renderer()
{
    renderer->add_render_pass(std::make_unique<ForwardPass>());

    auto point_light = std::make_unique<PointLight>(glm::vec3(1.0f), glm::vec3(1.0f));
    renderer->add_light(std::move(point_light));
}

void Application::run()
{
    window->set_mouse_visible(true);
    Device::enable_feature(GL_DEPTH_TEST);
    Device::set_vsync(false);


    init_scene();
    init_renderer();


    loop();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    window->terminate();
    Device::terminate();
}

void Application::render(float /*dt*/)
{
    Device::clear(true, glm::vec3(0.1f, 0.1f, 0.1f), true, false, 0);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    renderer->render();

    ImGui::Begin("Diagnostics");
    ImGui::Text("FPS: %d", static_cast<i32>(frame_timer.fps()));
    ImGui::PlotLines("Frame Time", frame_timer.lin().data(), static_cast<i32>(frame_timer.size()));
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::loop()
{

    while (!window->should_close())
    {
        frame_timer.update();

        Device::poll_events();
        cam_controller.update(*window, frame_timer.delta_time());

        render(frame_timer.delta_time());

        Device::process_errors();
        window->swap_buffers();

    }
}

} // namespace cppsim