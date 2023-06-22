#include "render/camera/camera.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include <glm/ext/matrix_transform.hpp>

#include <numbers>
#include <cmath>

namespace cppsim {


void Camera::set_view_matrix(const glm::vec3& position, const glm::vec3& look_at, const glm::vec3& up)
{
    view_matrix = glm::lookAt(position, look_at, up);
}

void Camera::set_perspective_projection(float fov, float aspect, float near, float far)
{
    proj_matrix = glm::perspective(fov, aspect, near, far);
}


void Camera::basis_vectors(glm::vec3& right, glm::vec3& up, glm::vec3& forward) const
{
    glm::mat3 transposed = glm::transpose(view_matrix);

    right = transposed[0];
    up = transposed[1];
    forward = transposed[2];
}


glm::vec3 Camera::extract_rotation() const
{
     // Columns should be divided by scale first, but scale is (1, 1, 1)
    glm::vec3 rotation(0.0f);
    float f = view_matrix[1][2];
    if (std::abs(std::abs(f) - 1.0f) < 0.00001f)
    {
        rotation.x = -f * static_cast<float>(std::numbers::pi) * 0.5f;
        rotation.y = std::atan2(-f * view_matrix[0][1], -f * view_matrix[0][0]);
        rotation.z = 0.0f;
    }
    else
    {
        rotation.x = -std::asin(f);
        float cosX = std::cos(rotation.x);
        rotation.y = std::atan2(view_matrix[0][2] / cosX, view_matrix[2][2] / cosX);
        rotation.z = std::atan2(view_matrix[1][0] / cosX, view_matrix[1][1] / cosX);
    }
    return rotation;
}


glm::vec3 Camera::extract_scale() const
{
     // Should return (1, 1, 1)
    return {view_matrix[0].length(), view_matrix[1].length(), view_matrix[2].length()};
}

glm::vec3 Camera::extract_translation() const
{
    // Keep only 3x3 rotation part of the matrix
    glm::mat3 transposed = glm::transpose(view_matrix);

    glm::vec3 inverse_translation = view_matrix[3];

    return transposed * -inverse_translation;
}


} // namespace cppsim