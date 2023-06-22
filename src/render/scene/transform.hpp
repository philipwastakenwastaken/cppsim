#pragma once
#include <memory>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <utility>


namespace cppsim {


class Transform
{

  public:
    Transform();

    inline glm::vec3 get_translation() const { return translation; }
    inline void set_translation(const glm::vec3& t) { translation = t; dirty = true; }

    inline glm::vec3 get_rotation() const { return rotation; }
    inline void set_rotation(const glm::vec3& r) { rotation = r; dirty = true; }

    inline glm::vec3 get_scale() const { return scale; }
    inline void set_scale(const glm::vec3& s) { scale = s; dirty = true; }

    inline std::shared_ptr<Transform> get_parent() const { return parent; }
    inline void set_parent(std::shared_ptr<Transform> p) { parent = std::move(p); dirty = true; }

    glm::mat4 translation_matrix() const;
    glm::mat4 rotation_matrix() const;
    glm::mat4 scale_matrix() const;

    glm::mat4 transform_matrix() const;

    bool is_dirty() const;

  private:
    glm::vec3 translation;
    glm::vec3 rotation;
    glm::vec3 scale;

    std::shared_ptr<Transform> parent = nullptr;

    // Cached matrix
    mutable glm::mat4 matrix;
    mutable bool dirty;
};


} // namespace cppsim