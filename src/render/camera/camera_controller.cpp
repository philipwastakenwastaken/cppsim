#include "render/camera/camera_controller.hpp"

#include <utility>
#include "glm/geometric.hpp"
#include "render/window.hpp"
#include "render/scene/scene_camera.hpp"

namespace cppsim {

CameraController::CameraController(std::shared_ptr<SceneCamera> c)
    : camera(std::move(c)), mouse_pos(0.0f), translation_speed(0.5f), rotation_speed(0.3f)
{}


void CameraController::update(const Window& window, float dt)
{
    if (first_update)
    {
        mouse_pos = window.mouse_position(true);
        first_update = false;
    }

    update_translation(window, dt);
    update_rotation(window, dt);

    camera->match_camera_to_transform();
}

void CameraController::update_translation(const Window& window, float dt)
{
    Transform& transform = *camera->get_transform();
    glm::vec3 translation = transform.get_translation();

    glm::vec3 input_translation(0.0f);

    if (window.is_key_pressed(GLFW_KEY_A))
        input_translation.x = -1.0f;
    else if (window.is_key_pressed(GLFW_KEY_D))
        input_translation.x = 1.0f;

    if (window.is_key_pressed(GLFW_KEY_W))
        input_translation.y = -1.0f;
    else if (window.is_key_pressed(GLFW_KEY_S))
        input_translation.y = 1.0f;

    if (window.is_key_pressed(GLFW_KEY_LEFT_SHIFT))
        input_translation.z = -1.0f;
    else if (window.is_key_pressed(GLFW_KEY_SPACE))
        input_translation.z = 1.0f;

    input_translation *= translation_speed * dt;

    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 forward;
    camera->get_camera()->basis_vectors(right, up, forward);

    translation += input_translation.x * right;
    translation += input_translation.y * forward;
    translation += input_translation.z * up;

    transform.set_translation(translation);
}

void CameraController::update_rotation(const Window& window, float /*unused*/)
{
    Transform& transform = *camera->get_transform();
    glm::vec3 rotation = transform.get_rotation();

    glm::vec2 mouse = window.mouse_position(true);
    glm::vec2 delta_mouse_position = mouse - mouse_pos;
    mouse_pos = mouse;

    glm::vec3 input_rotation(-delta_mouse_position.y, -delta_mouse_position.x, 0.0f);

    input_rotation *= rotation_speed;

    rotation += input_rotation;

    transform.set_rotation(rotation);
}

} // namespace cppsim