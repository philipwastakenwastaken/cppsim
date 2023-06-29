#pragma once
#include "core/core.hpp"
#include "core/log.hpp"
#include "core/hardware.hpp"

#include "render/lightning/light.hpp"

namespace cppsim {

class PointLight : public Light
{
  public:
    PointLight(const glm::vec3& c, const glm::vec3& p) : Light(c), position(p) {}

    [[nodiscard]] inline glm::vec3 get_position() const override { return position; }

  private:
    glm::vec3 position;
};


} // namespace cppsim
