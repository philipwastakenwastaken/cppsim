#include "render/window.hpp"
#include "GLFW/glfw3.h"
#include "core/core.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "render/opengl/vertex_array.hpp"
#include "render/opengl/vertex_buffer.hpp"
#include "render/opengl/gl_shader.hpp"

#include "util/rng.hpp"

namespace cppsim {

Window::Window()
{
    auto glfw_result = glfwInit();
    CPPSIM_ASSERT(glfw_result, "Could not initialize GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef CPPSIM_PLATFORM_MACOSX
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    window = glfwCreateWindow(window_width, window_height, "cppsim", nullptr, nullptr);

    CPPSIM_ASSERT(window, "Could not initialize GLFW window");

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    auto gl_version = gladLoadGL(glfwGetProcAddress);
    CPPSIM_ASSERT(gl_version, "Could not initalize glad");
    CPPSIM_TRACE("Loaded OpenGL version {0}.{1}", GLAD_VERSION_MAJOR(gl_version), GLAD_VERSION_MINOR(gl_version));


    int h = 0;
    int w = 0;
    glfwGetFramebufferSize(window, &w, &h);
    CPPSIM_TRACE("w: {0} h {1}", w, h);
    glViewport(0, 0, w, h);
}

Window::~Window() { glfwDestroyWindow(window); }


glm::vec2 Window::mouse_position(bool normalized) const
{
    double x = 0;
    double y = 0;
    glfwGetCursorPos(window, &x, &y);

    glm::vec2 ret(static_cast<float>(x), static_cast<float>(y));

    if (normalized)
    {
        ret.x = ret.x / window_width * 2.0f - 1.0f;
        ret.y = ret.y / window_height * 2.0f - 1.0f;
    }

    return ret;
}

Window::PressedState Window::key_state(int key_code) const
{
    return static_cast<PressedState>(glfwGetKey(window, key_code));
}

float Window::aspect_ratio() const { return window_width / static_cast<float>(window_height); }


void Window::set_mouse_visible(bool visible)
{
    glfwSetInputMode(window, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

} // namespace cppsim