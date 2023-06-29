#include "render/shaders/shader_updates.hpp"
#include <memory>
#include "render/lightning/point_light.hpp"


namespace cppsim {




void projection_model_view_transform(const std::shared_ptr<Shader>& shader, const glm::mat4& model, const std::shared_ptr<Camera>& camera)
{
    shader->bind();

    auto view_matrix = camera->get_view_matrix();
    auto project_matrix = camera->get_project_matrix();

    shader->set_uniform_mat4f("model", model);
    shader->set_uniform_mat4f("view", view_matrix);
    shader->set_uniform_mat4f("proj", project_matrix);
}



void point_light_update(const std::shared_ptr<Shader>& shader, const std::unique_ptr<Light>& light)
{
    shader->bind();

    shader->set_uniform3fv("light_pos", light->get_position());
    shader->set_uniform3fv("light_color", light->get_color());
}

}