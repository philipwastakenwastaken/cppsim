#pragma once

#include "render/scene/scene_node.hpp"
#include "render/camera/camera.hpp"

#include <memory>
#include <string>

namespace cppsim {

class SceneCamera : public SceneNode
{
  public:
    explicit SceneCamera(std::string_view s, std::shared_ptr<Camera> c);
    SceneCamera(std::string_view s, std::shared_ptr<Camera> c, std::shared_ptr<Transform> t);

    [[nodiscard]] std::shared_ptr<Camera> get_camera() { return camera; }
    [[nodiscard]] std::shared_ptr<const Camera> get_camera() const { return camera; }

    void match_camera_to_transform();
    void match_transform_to_camera();

  private:
    std::shared_ptr<Camera> camera;
};


} // namespace cppsim