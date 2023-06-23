#include "render/opengl/texture.hpp"
#include "core/core.hpp"
#include "core/log.hpp"

#include <glad/gl.h>

namespace cppsim {


Texture::Texture() { glGenTextures(1, &tex_id); }

Texture::~Texture() { glDeleteTextures(1, &tex_id); }

void Texture::bind() const { glBindTexture(GL_TEXTURE_2D, tex_id); }


void Texture::load_2d_texture(i32 width, i32 height, u8* data)
{
    CPPSIM_ASSERT(data != nullptr, "Texture data is null");

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


} // namespace cppsim