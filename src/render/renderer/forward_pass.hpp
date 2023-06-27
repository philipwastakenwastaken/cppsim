#pragma once
#include "core/core.hpp"
#include "core/log.hpp"
#include "core/hardware.hpp"
#include "render/renderer/render_pass.hpp"

namespace cppsim {

class ForwardPass : public RenderPass
{

  public:
    void render() override;

  private:
};


} // namespace cppsim
