#pragma once
#include "core/core.hpp"
#include "core/log.hpp"
#include "core/hardware.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "render/camera/camera.hpp"
#include "render/opengl/mesh.hpp"
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include "render/renderer/render_pass.hpp"
#include "render/scene/scene.hpp"
#include "render/scene/scene_model.hpp"
#include "render/lightning/light.hpp"
#include "render/shaders/shader_updates.hpp"

namespace cppsim {
using LightCollection = std::vector<std::unique_ptr<Light>>;



    class Renderer
{
  public:
    Renderer(SceneGraph& sc, std::shared_ptr<Camera> c) : scene_graph(sc), current_camera(std::move(c)) {}

    void update_light(const std::shared_ptr<Shader>& shader, const std::unique_ptr<Light>& light);
    void prepare_draw_call(const std::shared_ptr<SceneModel>& model);
    void add_render_pass(std::unique_ptr<RenderPass> pass);
    void render();

    [[nodiscard]] inline SceneGraph& get_scene_graph() { return scene_graph; }
    [[nodiscard]] inline const LightCollection& get_lights() { return lights; }

    inline void add_light(std::unique_ptr<Light> light) { lights.push_back(std::move(light)); }
    void add_update_transform_function(const std::shared_ptr<Shader>& shader, const UpdateTransformFunction& f);
    void add_update_light_function(const std::shared_ptr<Shader>& shader, const UpdateLightFunction& f);


  private:
    SceneGraph& scene_graph;
    LightCollection lights;
    std::shared_ptr<Camera> current_camera;
    std::vector<std::unique_ptr<RenderPass>> render_passes;

    std::unordered_map<std::shared_ptr<Shader>, UpdateTransformFunction> transform_update_functions;
    std::unordered_map<std::shared_ptr<Shader>, UpdateLightFunction> light_update_functions;
};


} // namespace cppsim
