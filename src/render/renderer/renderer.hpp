#pragma once
#include "core/core.hpp"
#include "core/log.hpp"
#include "core/hardware.hpp"
#include "render/camera/camera.hpp"
#include "render/opengl/mesh.hpp"
#include <memory>
#include <vector>
#include "render/renderer/render_pass.hpp"
#include "render/scene/scene.hpp"
#include "render/scene/scene_model.hpp"

namespace cppsim {

class Renderer
{
  public:
    Renderer(SceneGraph& sc, std::shared_ptr<Camera> c) : scene_graph(sc), current_camera(std::move(c)) {}

    void prepare_draw_call(const std::shared_ptr<SceneModel>& model);
    void add_render_pass(std::unique_ptr<RenderPass> pass);
    void render();

    [[nodiscard]] inline SceneGraph& get_scene_graph() { return scene_graph; }

  private:
    SceneGraph& scene_graph;
    std::vector<glm::vec4> world_matrices;
    std::shared_ptr<Camera> current_camera;
    std::vector<std::unique_ptr<RenderPass>> render_passes;
};


} // namespace cppsim
