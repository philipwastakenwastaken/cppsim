#include "index_buffer.hpp"

#include <glad/gl.h>

namespace cppsim {


IndexBuffer::IndexBuffer(const unsigned short* data, unsigned count) : count(count)
{
    glGenBuffers(1, &render_id); // gen vertex buffer id
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_id); // bind the newly generated id
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned short), data, GL_STATIC_DRAW); // load data into buffer
}

void IndexBuffer::delete_buffer()
{
    glDeleteBuffers(1, &render_id);
}

void IndexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, render_id); // for rebinding VBO
}

void IndexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0); // for unbinding VBO
}

} // namespace cppsim