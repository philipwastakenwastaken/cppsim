#include "render/window.hpp"
#include "GLFW/glfw3.h"
#include "core/core.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "render/opengl/vertex_array.hpp"
#include "render/opengl/vertex_buffer.hpp"
#include "render/opengl/gl_shader.hpp"

#include "util/rng.hpp"

namespace cppsim {

Window::Window(i32 w, i32 h): width(w), height(h)
{

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLContextVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLContextVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef CPPSIM_PLATFORM_MACOSX
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    window = glfwCreateWindow(w, h, WindowTitle, nullptr, nullptr);
    CPPSIM_ASSERT(window, "Could not initialize GLFW window");
}


glm::vec2 Window::mouse_position(bool normalized) const
{
    double x = 0;
    double y = 0;
    glfwGetCursorPos(window, &x, &y);

    glm::vec2 ret(static_cast<float>(x), static_cast<float>(y));

    if (normalized)
    {
        ret.x = ret.x / width * 2.0f - 1.0f;
        ret.y = ret.y / height * 2.0f - 1.0f;
    }

    return ret;
}

Window::PressedState Window::key_state(int key_code) const
{
    return static_cast<PressedState>(glfwGetKey(window, key_code));
}

float Window::aspect_ratio() const { return width / static_cast<float>(height); }


void Window::set_mouse_visible(bool visible)
{
    glfwSetInputMode(window, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

} // namespace cppsim