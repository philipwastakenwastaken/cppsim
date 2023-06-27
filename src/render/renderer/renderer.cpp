#include "render/renderer/renderer.hpp"
#include "render/renderer/render_pass.hpp"

namespace cppsim {

void Renderer::prepare_draw_call(const std::shared_ptr<SceneModel>& model)
{
    Mesh& mesh = model->get_mesh();
    auto shader = mesh.get_shader();
    shader->bind();

    auto trans_matrix = model->get_transform()->transform_matrix();
    auto pv_matrix = current_camera->project_view_matrix();
    trans_matrix = pv_matrix * trans_matrix;

    shader->set_uniform_mat4f("model", trans_matrix);
}

void Renderer::add_render_pass(std::unique_ptr<RenderPass> pass)
{
    pass->set_renderer(this);
    render_passes.push_back(std::move(pass));
}


void Renderer::render()
{


    for (auto& pass : render_passes)
    {
        pass->render();
    }


}




}