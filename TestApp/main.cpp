#include <NeoEngine.hpp>

#include "CustomShader.hpp"
#include "CustomRenderable.hpp"
#include "CustomSystem.hpp"

using namespace neo;

/* Systems */
RenderSystem * renderSystem;
CustomSystem * customSystem;

/* Shaders */
CustomShader * customShader;

/* Game object definitions */
struct Camera {
    GameObject *gameObject;
    SpatialComponent *spatial;
    CameraComponent *cameraComp;

    Camera(float fov, float near, float far, glm::vec3 pos) {
        gameObject = &NeoEngine::createGameObject();
        spatial = &NeoEngine::addComponent<SpatialComponent>(*gameObject, pos, glm::vec3(1.f));
        cameraComp = &NeoEngine::addComponent<CameraComponent>(*gameObject, fov, near, far, spatial);

        NeoEngine::addImGuiFunc([&]() {
            ImGui::Begin("Camera");

            float fov = cameraComp->getFOV();
            ImGui::SliderFloat("FOV", &fov, 0.f, 90.f);
            cameraComp->setFOV(fov);

            float near = cameraComp->getNear();
            float far = cameraComp->getFar();
            ImGui::SliderFloat("Near", &near, 0.f, 2.f);
            ImGui::SliderFloat("Far", &far, 10.f, 10000.f);
            cameraComp->setNearFar(near, far);

            glm::vec3 position = spatial->getPosition();
            ImGui::Text("Pos:     %0.2f, %0.2f, %0.2f", position.x, position.y, position.z);
            glm::vec3 lookDir  = cameraComp->getLookDir();
            ImGui::Text("lookDir: %0.2f, %0.2f, %0.2f", lookDir.x, lookDir.y, lookDir.z);
            ImGui::End();
        });
    }
};

struct Renderable {
    GameObject *gameObject;
    SpatialComponent *spatial;
    CustomRenderable *renderComponent;

    Renderable(std::string name, glm::vec3 p, float s, glm::mat3 o = glm::mat3()) {
        Mesh *mesh = Loader::getMesh(name);

        gameObject = &NeoEngine::createGameObject();
        spatial = &NeoEngine::addComponent<SpatialComponent>(*gameObject, p, glm::vec3(s), o);
        renderComponent = &NeoEngine::addComponent<CustomRenderable>(*gameObject, mesh, spatial);
    }

    void attachImGui(const std::string & name) {
        NeoEngine::addImGuiFunc([&]() {
            ImGui::Begin(name.c_str());
            glm::vec3 pos = spatial->getPosition();
            float scale = spatial->getScale().x;
            glm::vec3 u = spatial->getU();
            glm::vec3 v = spatial->getV();
            glm::vec3 w = spatial->getW();
            if (ImGui::SliderFloat3("Position", glm::value_ptr(pos), -10.f, 10.f)) {
                spatial->setPosition(pos);
            }
            if (ImGui::SliderFloat("Scale", &scale, 0.f, 10.f)) {
                spatial->setScale(glm::vec3(scale));
            }
            ImGui::SliderFloat("Spin", &renderComponent->spin, 0.f, 1.f);
            ImGui::SliderFloat3("RotationU", glm::value_ptr(u), 0.f, 1.f);
            ImGui::SliderFloat3("RotationV", glm::value_ptr(v), 0.f, 1.f);
            ImGui::SliderFloat3("RotationW", glm::value_ptr(w), 0.f, 1.f);
            spatial->setUVW(u, v, w);
            ImGui::End();
        });
    }
};

int main() {
    NeoEngine::init("TestApp", "res/", 1280, 720);
    NeoEngine::addImGuiFunc([&]() {
        ImGui::Begin("Stats");
        ImGui::Text("FPS: %d", NeoEngine::FPS);
        ImGui::Text("dt: %0.5f", NeoEngine::timeStep);
        if (ImGui::Button("VSync")) {
            Window::toggleVSync();
        }
        ImGui::End();
    });

    /* Init components */
    Camera camera(45.f, 0.01f, 100.f, glm::vec3(0, 0, 5));
    Renderable cube("cube.obj", glm::vec3(0.f), 1.f, glm::mat3(glm::rotate(glm::mat4(1.f), 0.707f, glm::vec3(1, 0, 0))));
    cube.attachImGui("Cube");
    
    /* Systems - order matters! */
    customSystem = &NeoEngine::addSystem<CustomSystem>();
    renderSystem = &NeoEngine::addSystem<RenderSystem>("shaders/", camera.cameraComp);

    /* Shaders */
    customShader = &renderSystem->addShader<CustomShader>("custom.vert", "custom.frag");

    /* Run */
    NeoEngine::initSystems();
    NeoEngine::run();

    return 0;
}
