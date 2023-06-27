#pragma once
#include "core/core.hpp"
#include "core/log.hpp"
#include "core/hardware.hpp"

namespace cppsim {

class Renderer;

class RenderPass
{

  public:
    RenderPass() = default;
    virtual ~RenderPass() = default;
    virtual void render() = 0;

    void set_renderer(Renderer* r);

  protected:
    Renderer* renderer;
};


} // namespace cppsim
