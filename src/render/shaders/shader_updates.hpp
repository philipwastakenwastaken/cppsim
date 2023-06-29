#pragma once

#include <functional>
#include "render/opengl/gl_shader.hpp"
#include "render/lightning/light.hpp"
#include "render/camera/camera.hpp"

namespace cppsim {

using UpdateTransformFunction = std::function<void(const std::shared_ptr<Shader>&, const glm::mat4&, const std::shared_ptr<Camera>&)>;
using UpdateLightFunction = std::function<void(const std::shared_ptr<Shader>&, const std::unique_ptr<Light>&)>;


void projection_model_view_transform(const std::shared_ptr<Shader>& shader, const glm::mat4& model, const std::shared_ptr<Camera>& camera);


void point_light_update(const std::shared_ptr<Shader>& shader, const std::unique_ptr<Light>& light);


}