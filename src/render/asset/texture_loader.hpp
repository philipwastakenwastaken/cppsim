#pragma once
#include "core/core.hpp"
#include "core/log.hpp"
#include "core/hardware.hpp"
#include "render/opengl/texture.hpp"

#include <string>

namespace cppsim {

class TextureLoader
{
  public:
     explicit TextureLoader(std::string_view p): path(p) {}
     Texture create_texture();

  private:
      std::string path;
};


} // namespace cppsim
