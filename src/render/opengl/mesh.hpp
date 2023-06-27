#pragma once
#include <utility>

#include "core/core.hpp"
#include "core/log.hpp"
#include "core/hardware.hpp"
#include "render/opengl/draw_call.hpp"
#include "render/opengl/gl_shader.hpp"
#include "render/opengl/texture.hpp"
#include "render/opengl/vertex_array.hpp"
#include "render/opengl/vertex_buffer.hpp"
#include "render/opengl/index_buffer.hpp"


namespace cppsim {


class Mesh
{

  public:
    Mesh(std::shared_ptr<Shader> s,
        const VertexArray& a,
        const VertexBuffer& b,
        const IndexBuffer& i,
        const DrawCall& d)
        : shader(std::move(s)), vao(a), vbo(b), ibo(i), draw_call(d)
    {}

    Mesh(const VertexArray& a, const VertexBuffer& b, const IndexBuffer& i) : vao(a), vbo(b), ibo(i) {}

    Mesh(const VertexArray& a, const VertexBuffer& b) : vao(a), vbo(b) {}


    void draw() const;
    void set_shader(std::shared_ptr<Shader> s) { shader = std::move(s); }
    void set_draw_call(const DrawCall& d) { draw_call = d; }
    void set_texture(const Texture& t) { texture = t; }
    void set_num_vertices(u64 v) { num_vertices = v; }

    [[nodiscard]] u64 get_num_vertices() const { return num_vertices; }


    std::shared_ptr<Shader> get_shader() { return shader; };


  private:
    std::shared_ptr<Shader> shader;
    VertexArray vao;
    VertexBuffer vbo;
    IndexBuffer ibo;
    Texture texture;

    u64 num_vertices;


    DrawCall draw_call;
};


} // namespace cppsim