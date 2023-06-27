#pragma once
#include <utility>

#include "core/core.hpp"
#include "core/log.hpp"
#include "core/hardware.hpp"
#include "render/opengl/mesh.hpp"
#include "render/scene/transform.hpp"
#include "render/scene/scene_node.hpp"

namespace cppsim {

class SceneModel : public SceneNode
{
  public:
    SceneModel(std::string_view s, std::shared_ptr<Transform> t, Mesh m)
        : SceneNode(s, std::move(t)), mesh(std::move(m))
    {}

    [[nodiscard]] inline Mesh& get_mesh() { return mesh; }
    [[nodiscard]] inline const Mesh& get_mesh() const { return mesh; }

  private:
    Mesh mesh;
};


} // namespace cppsim
