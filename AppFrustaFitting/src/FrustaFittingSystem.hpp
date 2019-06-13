#pragma once

#include "Systems/System.hpp"
#include "Engine.hpp"

#include "Component/CameraComponent/CameraComponent.hpp"
#include "Component/SpatialComponent/SpatialComponent.hpp"

using namespace neo;

class FrustaFittingSystem : public System {

public:
    FrustaFittingSystem() :
        System("FrustaFitting System")
    {}

    virtual void update(const float dt) override {
        auto mockOrthoCamera = Engine::getSingleComponent<MockOrthoComponent>();
        auto mockPerspectiveCamera = Engine::getSingleComponent<MockPerspectiveComponent>();
        if (!mockOrthoCamera || !mockPerspectiveCamera) {
            return;
        }
        auto orthoCamera = mockOrthoCamera->getGameObject().getComponentByType<CameraComponent>();
        auto perspectiveCamera = mockPerspectiveCamera->getGameObject().getComponentByType<CameraComponent>();
        if (!orthoCamera || !perspectiveCamera) {
            return;
        }
        float f = glm::sin(Util::getRunTime());
        float g = glm::cos(Util::getRunTime());
        perspectiveCamera->setLookDir(glm::vec3(f, -0.2f, g));
        orthoCamera->setLookDir(glm::vec3(f, -0.2f, g));
    }
};