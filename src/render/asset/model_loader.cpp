#include "render/asset/model_loader.hpp"
#include "core/hardware.hpp"
#include "render/opengl/mesh.hpp"
#include "render/opengl/vertex_array.hpp"
#include "render/opengl/vertex_buffer.hpp"
#include "render/opengl/vertex_buffer_layout.hpp"
#include "render/opengl/index_buffer.hpp"


#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>

namespace cppsim {


constexpr std::string_view MtlVertexPosition = "v";
constexpr std::string_view MtlVertexNormal = "vn";
constexpr std::string_view MtlTextureCoord = "vt";
constexpr std::string_view MtlFace = "f";

bool ModelLoader::found_vertex(const std::vector<float>& data,
    const std::vector<float>& vertex,
    u32 vertex_elements,
    u16& found_index) const
{
    bool found = false;

    for (u32 i = 0; i < data.size(); i += vertex_elements)
    {
        if (std::equal(data.begin() + i, data.begin() + i + vertex_elements, vertex.begin()))
        {
            found_index = static_cast<u16>(i);
            found = true;
            break;
        }
    }


    return found;
}

std::vector<float> ModelLoader::interleave_data()
{
    std::vector<float> data;
    std::vector<u16> indices;
    // auto unique = unique_vertices();

    u32 vertex_elements = 3 + has_normals * 3 + has_tex_coords * 2;
    u16 n_index = 0;
    u16 t_index = 0;

    for (const auto& face : mesh_faces)
    {

        for (u32 i = 0; i < FaceElements; i++)
        {
            std::vector<float> new_vertex;
            u16 v_index = face.position_indices[i];
            const auto& vertex_position = vertex_positions[v_index];
            new_vertex.push_back(vertex_position.x);
            new_vertex.push_back(vertex_position.y);
            new_vertex.push_back(vertex_position.z);


            if (has_tex_coords)
            {
                t_index = face.tex_indices[i];
                const auto& tex_coord = tex_coords[t_index];
                new_vertex.push_back(tex_coord.x);
                new_vertex.push_back(tex_coord.y);
            }

            if (has_normals)
            {
                n_index = face.normal_indices[i];
                const auto& vertex_normal = vertex_normals[n_index];
                new_vertex.push_back(vertex_normal.x);
                new_vertex.push_back(vertex_normal.y);
                new_vertex.push_back(vertex_normal.z);
            }


            if (has_faces)
            {
                u16 found_index = 0;
                if (!found_vertex(data, new_vertex, vertex_elements, found_index))
                {
                    indices.push_back(static_cast<u16>(data.size() / vertex_elements));
                    data.insert(data.end(), new_vertex.begin(), new_vertex.end());
                }
                else
                    indices.push_back(found_index / vertex_elements);
            }
        }
    }

    for (u32 i = 0; i < data.size(); i += vertex_elements)
    {
        for (u32 j = 0; j < vertex_elements; j++)
        {
            std::cout << data[i + j] << " ";
        }
        std::cout << '\n';
    }

    for (u32 i = 0; i < indices.size(); i += 1)
    {
            std::cout << indices[i] << " ";
    }
        std::cout << '\n';

    vertex_indices = indices;
    return data;
}


Mesh ModelLoader::to_mesh()
{

    u32 vertex_size = sizeof(float) * 3 + has_normals * sizeof(float) * 3 + has_tex_coords * sizeof(float) * 2;

    auto interleaved = interleave_data();
    u32 vertex_count = static_cast<u32>(interleaved.size());

    VertexBuffer vbo(static_cast<void*>(interleaved.data()), vertex_count, vertex_size);
    vbo.bind();

    VertexBufferLayout vbo_layout;
    vbo_layout.push<float>(3);

    if (has_tex_coords)
        vbo_layout.push<float>(2);

    if (has_normals)
        vbo_layout.push<float>(3);

    VertexArray vao;
    vao.bind();
    vao.add_buffer(vbo, vbo_layout);

    if (has_faces)
    {
        IndexBuffer ibo(vertex_indices.data(), static_cast<u32>(vertex_indices.size()));
        Mesh mesh(vao, vbo, ibo);
        mesh.set_num_vertices(vertex_indices.size());
        return mesh;
    }


    Mesh mesh(vao, vbo);
    mesh.set_num_vertices(vertex_positions.size());
    return mesh;
}


void ModelLoader::load_mtl()
{
    std::string type;

    float v1 = 0;
    float v2 = 0;
    float v3 = 0;

    u16 i1 = 0;
    u16 i2 = 0;
    u16 i3 = 0;

    std::ifstream file(model_path);

    while (file >> type)
    {
        if (type == MtlVertexPosition)
        {
            file >> v1;
            file >> v2;
            file >> v3;

            vertex_positions.emplace_back(v1, v2, v3);
        }
        else if (type == MtlVertexNormal)
        {
            file >> v1;
            file >> v2;
            file >> v3;

            vertex_normals.emplace_back(v1, v2, v3);
        }
        else if (type == MtlTextureCoord)
        {
            // TODO: support optional w coordinate
            file >> v1;
            file >> v2;

            tex_coords.emplace_back(v1, v2);
        }
        else if (type == MtlFace)
        {
            std::string token;
            std::getline(file, token);
            Face face;

            if (token.find("//") != std::string::npos)
            {
                token = std::regex_replace(token, std::regex("//"), " ");
                std::stringstream ss(token);

                for (u32 i = 0; i < FaceElements; i++)
                {
                    ss >> i1;
                    ss >> i2;
                    face.position_indices[i] = i1 - 1;
                    face.normal_indices[i] = i2 - 1;

                    CPPSIM_TRACE("{0} {1}", i1, i2);
                }
                mesh_faces.push_back(face);
            }
            else if (token.find('/') != std::string::npos)
            {
                token = std::regex_replace(token, std::regex("/"), " ");
                std::stringstream ss(token);

                for (u32 i = 0; i < FaceElements; i++)
                {
                    ss >> i1;
                    ss >> i2;
                    ss >> i3;
                    face.position_indices[i] = i1 - 1;
                    face.tex_indices[i] = i2 - 1;
                    face.normal_indices[i] = i3 - 1;

                    CPPSIM_TRACE("{0} {1} {2}", i1, i2, i3);
                }
            }
            else {
                std::stringstream ss(token);
                for (u32 i = 0; i < FaceElements; i++)
                {
                    ss >> i1;
                    face.position_indices[i] = i1 - 1;
                    CPPSIM_TRACE("{0}", i1);
                }

            }
            mesh_faces.push_back(face);
        }
    }


    has_tex_coords = !tex_coords.empty();
    has_normals = !vertex_normals.empty();
    has_faces = !mesh_faces.empty();
}


} // namespace cppsim