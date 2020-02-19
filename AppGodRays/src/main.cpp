#include <Engine.hpp>

#include "Renderer/Shader/PhongShader.hpp"
#include "Renderer/Shader/AlphaTestShader.hpp"
#include "Renderer/Shader/GammaCorrectShader.hpp"

#include "SunComponent.hpp"
#include "SunOccluderComponent.hpp"
#include "GodRaySunShader.hpp"
#include "GodRayOccluderShader.hpp"
#include "BlurShader.hpp"
#include "CombineShader.hpp"

#include "glm/gtc/matrix_transform.hpp"

using namespace neo;

/* Game object definitions */
struct Camera {
    CameraComponent *camera;
    Camera(float fov, float near, float far, glm::vec3 pos, float ls, float ms) {
        GameObject *gameObject = &Engine::createGameObject();
        Engine::addComponent<SpatialComponent>(gameObject, pos, glm::vec3(1.f));
        camera = &Engine::addComponentAs<PerspectiveCameraComponent, CameraComponent>(gameObject, near, far, fov, Window::getAspectRatio());
        Engine::addComponent<CameraControllerComponent>(gameObject, ls, ms);
    }
};

struct Light {
    Light(glm::vec3 pos, float scale, glm::vec3 col, glm::vec3 att) {
        auto& gameObject = Engine::createGameObject();
        Engine::addComponent<SpatialComponent>(&gameObject, pos, glm::vec3(scale));
        Engine::addComponent<LightComponent>(&gameObject, col, att);
        Engine::addComponent<SunComponent>(&gameObject);

        Engine::addImGuiFunc("Light", [&]() {
            auto light = Engine::getSingleComponent<LightComponent>();
            if (auto spatial = light->getGameObject().getComponentByType<SpatialComponent>()) {
                glm::vec3 pos = spatial->getPosition();
                if (ImGui::SliderFloat3("Position", glm::value_ptr(pos), -100.f, 100.f)) {
                    spatial->setPosition(pos);
                }
                float scale = spatial->getScale().x;
                if (ImGui::SliderFloat("Scale", &scale, 0.f, 100.f)) {
                    spatial->setScale(glm::vec3(scale));
                }
                ImGui::SliderFloat3("Color", glm::value_ptr(light->mColor), 0.f, 1.f);
                ImGui::SliderFloat3("Attenuation", glm::value_ptr(light->mAttenuation), 0.f, 1.f);
            }
        });
    }
};

struct Renderable {
    GameObject *gameObject;

    Renderable(Mesh *mesh, glm::vec3 position = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f), glm::vec3 rotation = glm::vec3(0.f)) {
        gameObject = &Engine::createGameObject();
        Engine::addComponent<MeshComponent>(gameObject, *mesh);
        Engine::addComponent<SpatialComponent>(gameObject, position, scale, rotation);
    }
};

int main() {
    EngineConfig config;
    config.APP_NAME = "GodRays";
    config.APP_RES = "res/";
    Engine::init(config);

    /* Game objects */
    Camera camera(45.f, 1.f, 100.f, glm::vec3(0, 0.6f, 5), 0.4f, 7.f);
    Engine::addComponent<MainCameraComponent>(&camera.camera->getGameObject());

    Light(glm::vec3(0.f, 2.f, -20.f), 12.f, glm::vec3(1.f), glm::vec3(0.6, 0.2, 0.f));

    /* Sponza object */
    {
        auto asset = Loader::loadMultiAsset("sponza.obj");

        GameObject& parent = Engine::createGameObject();
        Engine::addComponent<SpatialComponent>(&parent, glm::vec3(0.f), glm::vec3(0.2f));

        for (auto& a : asset) {
            auto& phong = Engine::addComponent<renderable::PhongRenderable>(&parent);
            auto& occluder = Engine::addComponent<SunOccluderComponent>(&parent);
            phong.mesh = a.mesh;
            occluder.mesh = a.mesh;
            phong.material = a.material;
            if (a.diffuseTexture) {
                phong.diffuseMap = a.diffuseTexture;
                occluder.alphaMap = a.diffuseTexture;
            }
            if (a.speculatTexture) {
                phong.specularMap = a.speculatTexture;
            }
            if (a.ambientTexture) {
                phong.ambientMap = a.ambientTexture;
            }
            if (a.displacementTexture) {
                phong.normalMap = a.displacementTexture;
            }
        }
    }

    /* Systems - order matters! */
    Engine::addSystem<CameraControllerSystem>();

    /* Init renderer */
    Renderer::init("shaders/");
    Renderer::addPreProcessShader<GodRaySunShader>("billboard.vert", "godraysun.frag");
    Renderer::addPreProcessShader<GodRayOccluderShader>("model.vert", "godrayoccluder.frag");
    Renderer::addPreProcessShader<BlurShader>("blur.vert", "blur.frag");
    Renderer::addSceneShader<PhongShader>();
    Renderer::addSceneShader<AlphaTestShader>();
    Renderer::addPostProcessShader<CombineShader>("combine.frag");
    Renderer::addPostProcessShader<GammaCorrectShader>();

    /* Attach ImGui panes */

    /* Run */
    Engine::run();
    return 0;
}
