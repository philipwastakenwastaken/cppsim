#include "render/renderer/renderer.hpp"
#include "core/core.hpp"
#include "render/renderer/render_pass.hpp"

namespace cppsim {

void Renderer::prepare_draw_call(const std::shared_ptr<SceneModel>& model)
{
    Mesh& mesh = model->get_mesh();
    auto shader = mesh.get_shader();
    auto trans_matrix = model->get_transform()->transform_matrix();

    CPPSIM_ASSERT(transform_update_functions.contains(shader), "Shader update function not found");
    transform_update_functions[shader](shader, trans_matrix, current_camera);
}

void Renderer::add_render_pass(std::unique_ptr<RenderPass> pass)
{
    pass->set_renderer(this);
    render_passes.push_back(std::move(pass));
}

void Renderer::update_light(const std::shared_ptr<Shader>& shader, const std::unique_ptr<Light>& light)
{
    if (light_update_functions.contains(shader))
    {
        light_update_functions[shader](shader, light);
    }
}

void Renderer::render()
{

    for (auto& pass : render_passes)
    {
        pass->render();
    }
}


void Renderer::add_update_transform_function(const std::shared_ptr<Shader>& shader, const UpdateTransformFunction& f)
{
    transform_update_functions[shader] = f;
}

void Renderer::add_update_light_function(const std::shared_ptr<Shader>& shader, const UpdateLightFunction& f)
{
    light_update_functions[shader] = f;
}


} // namespace cppsim