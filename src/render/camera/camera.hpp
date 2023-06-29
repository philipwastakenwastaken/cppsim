#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>


namespace cppsim {


class Camera
{

  public:
    Camera(): view_matrix(glm::mat4(1.0f)), proj_matrix(glm::mat4(1.0f)) {}

    void set_view_matrix(const glm::vec3& position,
        const glm::vec3& look_at,
        const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

    void set_view_matrix(const glm::mat4& m) { view_matrix = m; }

    void set_perspective_projection(float fov, float aspect, float near, float far);

    [[nodiscard]] inline glm::mat4 project_view_matrix() { return proj_matrix * view_matrix; }
    [[nodiscard]] inline glm::mat4 get_project_matrix() { return proj_matrix; }
    [[nodiscard]] inline glm::mat4 get_view_matrix() { return view_matrix; }



    void basis_vectors(glm::vec3& right, glm::vec3& up, glm::vec3& forward) const;

    [[nodiscard]] glm::vec3 extract_rotation() const;
    [[nodiscard]] glm::vec3 extract_scale() const;
    [[nodiscard]] glm::vec3 extract_translation() const;


  private:
    glm::mat4 view_matrix;
    glm::mat4 proj_matrix;
};


} // namespace cppsim