#include "render/opengl/texture.hpp"

#include <glad/gl.h>

namespace cppsim {


Texture::Texture() { glGenTextures(1, &tex_id); }

Texture::~Texture() { glDeleteTextures(1, &tex_id); }

void Texture::bind() const { glBindTexture(GL_TEXTURE_2D, tex_id); }


void load_2d_texture(i32 width, i32 height, u8* data)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}


} // namespace cppsim