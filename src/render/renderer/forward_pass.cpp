#include "render/renderer/forward_pass.hpp"
#include "core/log.hpp"
#include "render/renderer/renderer.hpp"
#include <memory>


namespace cppsim {



void ForwardPass::render()
{
    auto& scene_graph = renderer->get_scene_graph();

    for (const auto& node : scene_graph)
    {
        auto model = std::static_pointer_cast<SceneModel>(node);

        renderer->prepare_draw_call(model);
        model->get_mesh().draw();
    }
}
}