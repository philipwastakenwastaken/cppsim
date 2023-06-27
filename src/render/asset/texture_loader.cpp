#define STB_IMAGE_IMPLEMENTATION
#include "render/asset/texture_loader.hpp"
#include "stb/stb_image.h"

namespace cppsim {



Texture TextureLoader::create_texture()
{
    Texture tex;
    tex.bind();
    i32 w = 0;
    i32 h = 0;
    i32 n = 0;
    u8* tex_data = stbi_load(path.c_str(), &w, &h, &n, 0);
    tex.load_2d_texture(w, h, tex_data);
    stbi_image_free(tex_data);
    return tex;
}



}