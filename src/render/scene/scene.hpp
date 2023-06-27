#pragma once
#include "core/core.hpp"
#include "core/log.hpp"
#include "core/hardware.hpp"
#include "render/scene/scene_camera.hpp"
#include "render/scene/scene_node.hpp"
#include <memory>
#include <vector>

namespace cppsim {

using SceneGraph = std::vector<std::shared_ptr<SceneNode>>;

class Scene
{
  public:
    explicit Scene(std::shared_ptr<SceneCamera> sc): scene_camera(std::move(sc)) {}


    inline void add_node(const std::shared_ptr<SceneNode>& node) { scene_graph.push_back(node); }

    inline SceneGraph& get_scene_graph() { return scene_graph; }
    inline std::shared_ptr<SceneCamera> get_scene_camera() { return scene_camera; }


  private:
    SceneGraph scene_graph;
    std::shared_ptr<SceneCamera> scene_camera;
};


} // namespace cppsim
