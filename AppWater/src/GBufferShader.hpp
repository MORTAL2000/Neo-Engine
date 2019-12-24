#pragma once

#include "Shader/Shader.hpp"
#include "GLObjects/GLHelper.hpp"
#include "Window/Window.hpp"

#include "Messaging/Messenger.hpp"

using namespace neo;

class GBufferShader : public Shader {

    public:

        GBufferShader(const std::string &vert, const std::string &frag) :
            Shader("GBufferShader", vert, frag) {

            // Create gbuffer 
            auto gbuffer = Library::getFBO("gbuffer");
            gbuffer->generate();

            // Format for color buffers
            TextureFormat format = { GL_RGBA, GL_RGBA, GL_NEAREST, GL_CLAMP_TO_EDGE };
            gbuffer->attachColorTexture(Window::getFrameSize(), format); // world
            gbuffer->attachColorTexture(Window::getFrameSize(), format); // normal
            gbuffer->attachColorTexture(Window::getFrameSize(), format); // diffuse
            gbuffer->attachDepthTexture(Window::getFrameSize(), GL_NEAREST, GL_CLAMP_TO_EDGE); // depth
            gbuffer->initDrawBuffers();

            // Handle frame size changing
            Messenger::addReceiver<WindowFrameSizeMessage>(nullptr, [&](const Message &msg) {
                const WindowFrameSizeMessage & m(static_cast<const WindowFrameSizeMessage &>(msg));
                glm::uvec2 frameSize = (static_cast<const WindowFrameSizeMessage &>(msg)).frameSize;
                Library::getFBO("gbuffer")->resize(frameSize);
            });
        }

        virtual void render(const CameraComponent &camera) override {
            auto fbo = Library::getFBO("gbuffer");
            fbo->bind();
            CHECK_GL(glClearColor(0.f, 0.f, 0.f, 1.f));
            CHECK_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            bind();
            loadUniform("P", camera.getProj());
            loadUniform("V", camera.getView());

            for (auto& renderable : Engine::getComponentTuples<MeshComponent, SpatialComponent>()) {
                loadUniform("M", renderable->get<SpatialComponent>()->getModelMatrix());

                /* Bind diffuse map or material */
                auto matComp = renderable->mGameObject.getComponentByType<MaterialComponent>();
                if (matComp) {
                    loadUniform("ambient", matComp->mAmbient);
                }
                if (auto diffMap = renderable->mGameObject.getComponentByType<DiffuseMapComponent>()) {
                    loadTexture("diffuseMap", diffMap->mTexture);
                    loadUniform("useDiffuseMap", true);
                }
                else {
                    loadUniform("useDiffuseMap", false);
                    if (matComp) {
                        loadUniform("diffuseMaterial", matComp->mDiffuse);
                    }
                }

                /* Bind normal map */
                auto normalMap = renderable->mGameObject.getComponentByType<neo::NormalMapComponent>();
                if (normalMap) {
                    loadTexture("normalMap", normalMap->mTexture);
                    loadUniform("useNormalMap", true);
                }
                else {
                    loadUniform("useNormalMap", false);
                    loadUniform("N", renderable->get<SpatialComponent>()->getNormalMatrix());
                }

                /* DRAW */
                renderable->get<MeshComponent>()->getMesh().draw();
            }

            unbind();
    }
};