#include "render/opengl/draw_call.hpp"
#include "glad/gl.h"


namespace cppsim {



void DrawCall::draw() const
{
    CPPSIM_ASSERT(primitive != Primitive::Invalid, "Invalid primitive in draw call");
    glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(polygon_mode));
    glDrawElements(static_cast<GLenum>(primitive), count, GL_UNSIGNED_SHORT, nullptr);
}




}