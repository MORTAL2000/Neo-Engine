#pragma once

#include "Component/Component.hpp"
#include "GameObject/GameObject.hpp"

#include <glm/glm.hpp>

namespace neo {

    class MouseRayComponent : public Component {
    public:
        glm::vec3 mPosition;
        glm::vec3 mDirection;
        MouseRayComponent(GameObject *go) :
            Component(go),
            mPosition(0.f),
            mDirection(0.f)
        {}
    };

}