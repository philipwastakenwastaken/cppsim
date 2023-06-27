#include "render/opengl/device.hpp"
#include "render/window.hpp"


namespace cppsim {

Device* Device::instance;

void Device::init()
{
    CPPSIM_ASSERT(instance == nullptr, "Device already initialized");
    instance = new Device();

    auto glfw_result = glfwInit();
    CPPSIM_ASSERT(glfw_result, "Could not initialize GLFW");
}

void Device::terminate() { glfwTerminate(); }

void Device::set_current_window(Window& window)
{
    auto* internal_window = window.internal_window();
    glfwMakeContextCurrent(internal_window);

    auto gl_version = gladLoadGL(glfwGetProcAddress);
    CPPSIM_ASSERT(gl_version, "Could not initalize glad");
    CPPSIM_TRACE("Loaded OpenGL version {0}.{1}", GLAD_VERSION_MAJOR(gl_version), GLAD_VERSION_MINOR(gl_version));

    glfwSetFramebufferSizeCallback(internal_window, frame_buffer_resized);
}

void Device::set_viewport(i32 width, i32 height) { glViewport(0, 0, width, height); }


void Device::frame_buffer_resized(GLFWwindow* /*window*/, i32 width, i32 height) { set_viewport(width, height); }

void Device::set_vsync(bool enable)
{
    i32 flag = static_cast<i32>(enable);
    glfwSwapInterval(flag);
}


void Device::process_errors()
{
    GLenum err = 0;
    while ((err = glGetError()) != GL_NO_ERROR)
        CPPSIM_TRACE(err);
}

void Device::enable_feature(u32 feature) { glEnable(feature); }

void Device::disable_feature(u32 feature) { glDisable(feature); }

void Device::poll_events() { glfwPollEvents(); }

void Device::clear(const glm::vec3& color) { clear(true, color, false, false, 0); }

void Device::clear(bool clear_color, const glm::vec3& color, bool clear_depth, bool clear_stencil, i32 stencil)
{
    u32 clear_mask = 0;
    if (clear_color)
    {
        glClearColor(color[0], color[1], color[2], 1.0f);
        clear_mask |= GL_COLOR_BUFFER_BIT;
    }

    if (clear_depth)
        clear_mask |= GL_DEPTH_BUFFER_BIT;

    if (clear_stencil)
    {
        glClearStencil(stencil);
        clear_mask |= GL_STENCIL_BUFFER_BIT;
    }

    glClear(clear_mask);
}


} // namespace cppsim