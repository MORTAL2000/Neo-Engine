#pragma once

#include "Component/Component.hpp"

namespace neo {
    namespace renderable {
        class SkyboxComponent : public Component {
        public:
            SkyboxComponent(GameObject *go) :
                Component(go)
            {}
        };
    }
}