#pragma once
#include "core/core.hpp"
#include "core/log.hpp"
#include "core/hardware.hpp"

#include "glm/vec3.hpp"

namespace cppsim {

class Light
{

  public:
    explicit Light(const glm::vec3& c): color(c) {}
    virtual ~Light() = default;

    [[nodiscard]] const glm::vec3& get_color() const { return color; }

    [[nodiscard]] inline virtual glm::vec3 get_position() const { return glm::vec3(0); }



  protected:
      glm::vec3 color;

};


} // namespace cppsim
