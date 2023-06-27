#pragma once
#include "core/core.hpp"
#include "core/log.hpp"
#include "core/hardware.hpp"
#include "render/camera/camera_controller.hpp"
#include "render/window.hpp"
#include "render/renderer/renderer.hpp"
#include "render/scene/scene.hpp"

#include <memory>

namespace cppsim {

class Application
{
  public:
      Application();

      void run();
  private:
      void init_scene();
      void init_renderer();

      void loop();
      void render(float dt);

      Window* window;
      CameraController cam_controller;
      std::unique_ptr<Scene> scene;
      std::unique_ptr<Renderer> renderer;
};


} // namespace cppsim
