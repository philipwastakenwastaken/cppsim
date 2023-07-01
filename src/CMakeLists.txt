option(CPPSIM_ENABLE_EXCEPTIONS "Enable Exceptions" OFF)
option(CPPSIM_ENABLE_RTTI "Enable Run-time Type Information" OFF)
option(CPPSIM_ENABLE_POPCNT "Enable POPCNT hardware instuction" OFF)
option(CPPSIM_ENABLE_PEXT "Enable PEXT hardware instuction" OFF)
option(CPPSIM_ENABLE_ALL_EXTENSIONS "Enable all instruction set extensions available for the host" OFF)


# First build a library containing the source files
add_library(cppsimlib STATIC render/window.cpp
                           render/opengl/vertex_buffer.cpp
                           render/opengl/vertex_buffer_layout.cpp
                           render/opengl/vertex_array.cpp
                           render/opengl/texture.cpp
                           render/opengl/mesh.cpp
                           render/opengl/device.cpp
                           render/opengl/draw_call.cpp
                           render/opengl/index_buffer.cpp
                           render/renderer/renderer.cpp
                           render/renderer/render_pass.cpp
                           render/renderer/forward_pass.cpp
                           render/asset/model_loader.cpp
                           render/asset/texture_loader.cpp
                           render/shaders/shader_updates.cpp
                           render/camera/camera.cpp
                           render/camera/camera_controller.cpp
                           render/scene/transform.cpp
                           render/scene/scene_camera.cpp
                           render/opengl/gl_shader.cpp
                           application.cpp
                           util/timer.cpp
                           util/frame_timer.cpp
                           util/rng.cpp
                           core/log.cpp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")


# Now link the source library publicly against spdlog
target_link_libraries(cppsimlib PUBLIC
                              project_options
                              project_warnings
                              glfw
                              glad
                              glm
                              stb
                              imgui
                              spdlog::spdlog)

# Prefer pthreads on Unix.
if (UNIX)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(cppsimlib PRIVATE Threads::Threads)
endif()

# Exceptions
if (NOT CPPSIM_ENABLE_EXCEPTIONS)
	if (NOT MSVC)
		target_compile_options(cppsimlib PUBLIC -fno-exceptions)
	endif()
endif()

# RTTI
if (NOT CPPSIM_ENABLE_RTTI)
    if (NOT MSVC)
        target_compile_options(cppsimlib PUBLIC -fno-rtti)
    endif()
endif()


# Instruction set extensions
if (CPPSIM_ENABLE_POPCNT AND NOT CPPSIM_ENABLE_ALL_EXTENSIONS)
    add_compile_definitions(CPPSIM_ENABLE_POPCNT)
	if (NOT MSVC)
		target_compile_options(cppsimlib PUBLIC -mpopcnt)
	endif()
endif()
if (CPPSIM_ENABLE_PEXT AND NOT CPPSIM_ENABLE_ALL_EXTENSIONS)
    add_compile_definitions(CPPSIM_ENABLE_PEXT)
	if (NOT MSVC)
		target_compile_options(cppsimlib PUBLIC -mbmi2)
	endif()
endif()

if (CPPSIM_ENABLE_ALL_EXTENSIONS)
    add_compile_definitions(CPPSIM_ENABLE_POPCNT)
    add_compile_definitions(CPPSIM_ENABLE_PEXT)
	if (NOT MSVC)
		target_compile_options(cppsimlib PUBLIC -march=native)
	endif()
endif()

# Create the final executable
add_executable(cppsim main.cpp)
target_link_libraries(cppsim PRIVATE
                            project_options
                            project_warnings
                            cppsimlib)