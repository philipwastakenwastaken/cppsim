#include "render/renderer/forward_pass.hpp"
#include "core/log.hpp"
#include "render/lightning/point_light.hpp"
#include "render/renderer/renderer.hpp"
#include <memory>


namespace cppsim {



void ForwardPass::render()
{
    auto& scene_graph = renderer->get_scene_graph();

    for (const auto& node : scene_graph)
    {
        auto model = std::static_pointer_cast<SceneModel>(node);
        auto shader = model->get_mesh().get_shader();
        renderer->prepare_draw_call(model);

        for (const auto& light : renderer->get_lights())
        {
            renderer->update_light(shader, light);
            model->get_mesh().draw();
        }

    }
}
}