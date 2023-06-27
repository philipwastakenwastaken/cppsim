#pragma once

#include "core/core.hpp"
#include "core/hardware.hpp"


namespace cppsim {


class Texture
{

  public:
    Texture();
    ~Texture() = default;

    void bind() const;

    void load_2d_texture(i32 width, i32 height, u8* data);


  private:
    u32 tex_id;
};


} // namespace cppsim