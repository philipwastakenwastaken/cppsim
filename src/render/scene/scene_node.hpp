#pragma once

#include <memory>
#include <string>
#include <utility>

#include "render/scene/transform.hpp"
#include "core/log.hpp"
#include "core/core.hpp"


namespace cppsim {


class SceneNode {

    public:

        explicit SceneNode(std::string_view s): name(s), transform(std::make_shared<Transform>()) {}
        SceneNode(std::string_view s, std::shared_ptr<Transform> t): name(s), transform(std::move(t)) {}

        [[nodiscard]] inline std::shared_ptr<const Transform> get_transform() const { return transform; }
        [[nodiscard]] inline std::shared_ptr<Transform> get_transform() { return transform; }

    protected:

    std::string name;
    std::shared_ptr<Transform> transform;


};




}