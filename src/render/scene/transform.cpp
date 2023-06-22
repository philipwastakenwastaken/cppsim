#include "render/scene/transform.hpp"
#include "glm/ext/matrix_transform.hpp"


namespace cppsim {

Transform::Transform() : translation(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1), matrix(1.0f), dirty(false)
{
}

glm::mat4 Transform::translation_matrix() const
{
    return glm::translate(glm::identity<glm::mat4>(), translation);
}

glm::mat4 Transform::rotation_matrix() const
{
    auto m = glm::identity<glm::mat4>();
    m = glm::rotate(m, rotation.y, glm::vec3(0, 1, 0));
    m = glm::rotate(m, rotation.x, glm::vec3(1, 0, 0));
    m = glm::rotate(m, rotation.z, glm::vec3(0, 0, 1));
    return m;
}

glm::mat4 Transform::scale_matrix() const
{
    return glm::scale(glm::identity<glm::mat4>(), scale);
}

glm::mat4 Transform::transform_matrix() const
{
    if (is_dirty())
    {
        matrix = translation_matrix() * rotation_matrix() * scale_matrix();
        if (parent)
        {
            matrix = parent->transform_matrix() * matrix;
        }
        dirty = false;
    }
    return matrix;
}

bool Transform::is_dirty() const
{
    return dirty || (parent && parent->is_dirty());
}


}