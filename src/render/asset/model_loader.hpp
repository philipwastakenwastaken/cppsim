#pragma once
#include "core/core.hpp"
#include "core/log.hpp"
#include "core/hardware.hpp"

#include "render/opengl/mesh.hpp"

#include <string>
#include <vector>
#include <array>

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

#include "glm/ext/vector_int3.hpp"


namespace cppsim {

constexpr u64 FaceElements = 3;
struct Face
{
    std::array<u16, FaceElements> tex_indices;
    std::array<u16, FaceElements> position_indices;
    std::array<u16, FaceElements> normal_indices;
};

class ModelLoader
{
  public:
    explicit ModelLoader(std::string_view path) : model_path(path) { load_mtl(); }

    [[nodiscard]] Mesh to_mesh();

  private:
    void load_mtl();
    [[nodiscard]] u64 number_of_vertices() const { return vertex_positions.size(); }
    [[nodiscard]] std::vector<float> interleave_data();
    [[nodiscard]] std::vector<glm::uvec3> unique_vertices() const;

    [[nodiscard]] bool found_vertex(const std::vector<float>& data,
        const std::vector<float>& vertex,
        u32 vertex_elements,
        u16& found_index) const;

    std::string model_path;

    std::vector<glm::vec3> vertex_positions;
    std::vector<glm::vec3> vertex_normals;
    std::vector<glm::vec2> tex_coords;
    std::vector<Face> mesh_faces;
    std::vector<u16> vertex_indices;

    bool has_tex_coords = false;
    bool has_normals = false;
    bool has_faces = false;
};


} // namespace cppsim