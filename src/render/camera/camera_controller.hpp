#pragma once

#include <glm/vec2.hpp>
#include <memory>

namespace cppsim {



class SceneCamera;
class Window;

class CameraController {

    public:
        CameraController() = default;
        explicit CameraController(std::shared_ptr<SceneCamera> c);


        void update(Window& window, float dt);


    private:
        void update_translation(const Window& window, float dt);
        void update_rotation(const Window& window, float dt);


        std::shared_ptr<SceneCamera> camera;
        glm::vec2 mouse_pos;

        float translation_speed;
        float rotation_speed;

        bool first_update = true;
        bool enabled = false;
        bool m_enable_pressed = false;





};



}