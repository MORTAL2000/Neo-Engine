#pragma once

#include "Systems/System.hpp"

#include "Component/CameraComponent/CameraComponent.hpp"

namespace neo {

    class FrustumBoundsSystem : public System {
    public:
        FrustumBoundsSystem() :
            System("FrustumBounds System")
        {}
    
        virtual void update(const float dt) override;
    };
}
