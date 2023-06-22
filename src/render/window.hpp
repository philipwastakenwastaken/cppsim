#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "core/hardware.hpp"
#include "core/log.hpp"
#include "core/core.hpp"

namespace cppsim {

class Window
{
  public:
    Window();
    ~Window();

    void swap_buffers() { glfwSwapBuffers(window); }
    void poll_events() { glfwPollEvents(); }
    [[nodiscard]] bool should_close() const { return glfwWindowShouldClose(window); }

    [[nodiscard]] GLFWwindow* get_window() { return window; }

    [[nodiscard]] float aspect_ratio() const;

    enum class PressedState {
        Pressed = GLFW_PRESS,
        Released = GLFW_RELEASE,
        Repeated = GLFW_REPEAT,
    };


    void set_mouse_visible(bool visible);

    // Helper functions to query the pressed state of a key
    [[nodiscard]] bool is_key_pressed(int key_code) const { return  key_state(key_code) == PressedState::Pressed; }
    [[nodiscard]] bool is_key_released(int key_code) const { return key_state(key_code) == PressedState::Released; }
    [[nodiscard]] bool is_key_repeated(int key_code) const { return key_state(key_code) == PressedState::Repeated; }

    [[nodiscard]] glm::vec2 mouse_position(bool normalized) const;

  private:
    // Get the pressed state of a key


    [[nodiscard]] PressedState key_state(int key_code) const;
    GLFWwindow* window = nullptr;

    i32 window_width = 500;
    i32 window_height = 500;
};

} // namespace cppsim