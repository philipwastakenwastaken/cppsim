#include "render/scene/scene_camera.hpp"
#include "core/core.hpp"
#include "core/log.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace cppsim {

SceneCamera::SceneCamera(std::string_view s, std::shared_ptr<Camera> c) : SceneNode(s), camera(std::move(c))
{
    match_transform_to_camera();
}

SceneCamera::SceneCamera(std::string_view s, std::shared_ptr<Camera> c, std::shared_ptr<Transform> t)
    : SceneNode(s, std::move(t)), camera(std::move(c))
{
    match_camera_to_transform();
}

void SceneCamera::match_camera_to_transform()
{
    if (camera && transform)
    {
        CPPSIM_ASSERT(transform->get_scale() == glm::vec3(1.0f), "Camera scale must be 1");

        glm::mat4 matrix = glm::transpose(transform->rotation_matrix());
        matrix = glm::translate(matrix, -transform->get_translation());
        camera->set_view_matrix(matrix);
    }
}

void SceneCamera::match_transform_to_camera()
{
     if (camera && transform)
    {
        transform->set_translation(camera->extract_translation());
        transform->set_rotation(camera->extract_rotation());
        transform->set_scale(glm::vec3(1.0f));
    }
}


} // namespace cppsim