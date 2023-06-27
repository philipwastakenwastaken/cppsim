#pragma once
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "core/core.hpp"
#include "core/log.hpp"
#include "core/hardware.hpp"
#include "glm/ext/vector_float3.hpp"
#include <memory>

namespace cppsim {

class Window;

class Device
{

  public:

    static void init();
    static void terminate();

    static void set_current_window(Window& window);
    static void poll_events();
    static void set_vsync(bool enable);
    static void set_viewport(i32 width, i32 height);
    static void enable_feature(u32 feature);
    static void disable_feature(u32 feature);
    static void process_errors();

    static void clear(const glm::vec3& color);
    static void clear(bool clear_color, const glm::vec3& color, bool clear_depth, bool clear_stencil, i32 stencil);


  private:
    static void frame_buffer_resized(GLFWwindow* window, i32 width, i32 height);


    bool context_loaded = false;
    static Device* instance;

};


} // namespace cppsim
