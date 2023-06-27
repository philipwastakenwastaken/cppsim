#include "render/opengl/mesh.hpp"


namespace cppsim {


void Mesh::draw() const
{
    shader->bind();
    texture.bind();
    vao.bind();

    draw_call.draw();
}




}