#include "RenderableComponent.hpp"

#include "NeoEngine.hpp"

namespace neo {

    RenderableComponent::RenderableComponent(GameObject &go, Mesh *mesh, Material *mat) :
        Component(go),
        mesh(mesh),
        material(mat)
    {}

    void RenderableComponent::init() {
        isInit = true;
        RenderSystem & rSystem = NeoEngine::getSystem<RenderSystem>();
        for (auto shaderT : shaderTypes) {
            rSystem.attachCompToShader(shaderT, this);
        }
    }

    void RenderableComponent::kill() {
        RenderSystem & rSystem = NeoEngine::getSystem<RenderSystem>();
        for (auto shaderT : shaderTypes) {
            rSystem.detachCompFromShader(shaderT, this);
        }
    }

}
