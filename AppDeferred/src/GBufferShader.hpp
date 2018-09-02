#pragma once

#include "Shader/Shader.hpp"
#include "GLHelper/GLHelper.hpp"
#include "MasterRenderer/MasterRenderer.hpp"
#include "Window/Window.hpp"

using namespace neo;

class GBufferShader : public Shader {

    public:

        GBufferShader(const std::string &vert, const std::string &frag) :
            Shader("GBufferShader", vert, frag) {
            // Position buffer 
            Texture2D *positionBuffer = new Texture2D;
            positionBuffer->width = Window::getFrameSize().x;
            positionBuffer->height = Window::getFrameSize().y;
            positionBuffer->components = 3;
            positionBuffer->upload(GL_RGBA, GL_RGBA, GL_NEAREST, GL_REPEAT);

            // Normal buffer
            Texture2D *normalBuffer = new Texture2D;
            normalBuffer->width = Window::getFrameSize().x;
            normalBuffer->height = Window::getFrameSize().y;
            normalBuffer->components = 3;
            normalBuffer->upload(GL_RGBA, GL_RGBA, GL_NEAREST, GL_REPEAT);

            // Diffuse 
            Texture2D *diffuseBuffer = new Texture2D;
            diffuseBuffer->width = Window::getFrameSize().x;
            diffuseBuffer->height = Window::getFrameSize().y;
            diffuseBuffer->components = 4;
            diffuseBuffer->upload(GL_RGBA, GL_RGBA, GL_NEAREST, GL_REPEAT);

            // Specular
            Texture2D *specularBuffer = new Texture2D;
            specularBuffer->width = Window::getFrameSize().x;
            specularBuffer->height = Window::getFrameSize().y;
            specularBuffer->components = 4;
            specularBuffer->upload(GL_RGBA, GL_RGBA, GL_NEAREST, GL_REPEAT);

            // Depth buffer
            Texture2D *depthBuffer = new Texture2D;
            depthBuffer->width = Window::getFrameSize().x;
            depthBuffer->height = Window::getFrameSize().y;
            depthBuffer->components = 1;
            depthBuffer->upload(GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_NEAREST, GL_REPEAT);

            // Create gbuffer 
            auto gbuffer = MasterRenderer::getFBO("gbuffer");
            gbuffer->generate();
            gbuffer->attachColorTexture(*positionBuffer);
            gbuffer->attachColorTexture(*normalBuffer);
            gbuffer->attachColorTexture(*diffuseBuffer);
            gbuffer->attachColorTexture(*specularBuffer);
            gbuffer->attachDepthTexture(*depthBuffer);
            gbuffer->initDrawBuffers();
        }

        virtual void render(const CameraComponent &camera) override {
            auto fbo = MasterRenderer::getFBO("gbuffer");
            fbo->bind();
            CHECK_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
            CHECK_GL(glViewport(0, 0, fbo->textures[0]->width, fbo->textures[0]->height));  // TODO : windowresize receiver

            bind();
            loadUniform("P", camera.getProj());
            loadUniform("V", camera.getView());

            for (auto model : MasterRenderer::getRenderables<GBufferShader, RenderableComponent>()) {
                loadUniform("M", model->getGameObject().getSpatial()->getModelMatrix());

                /* Bind mesh */
                const Mesh & mesh(model->getMesh());
                CHECK_GL(glBindVertexArray(mesh.vaoId));
                CHECK_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.eleBufId));

                /* Bind diffuse map or material */
                auto matComp = model->getGameObject().getComponentByType<MaterialComponent>();
                auto diffMap = model->getGameObject().getComponentByType<DiffuseMapComponent>();
                if (diffMap) {
                    diffMap->getTexture().bind();
                    loadUniform("useDiffuseMap", true);
                    loadUniform("diffuseMap", diffMap->getTexture().textureId);
                }
                else {
                    loadUniform("useDiffuseMap", false);
                    if (matComp) {
                        loadUniform("diffuseMaterial", matComp->getMaterial().diffuse);
                    }
                }

                /* Bind specular map */
                auto specularMap = model->getGameObject().getComponentByType<SpecularMapComponent>();
                if (specularMap) {
                    specularMap->getTexture().bind();
                    loadUniform("useSpecularMap", true);
                    loadUniform("specularMap", specularMap->getTexture().textureId);
                }
                else {
                    loadUniform("useSpecularMap", false);
                    if (matComp) {
                        loadUniform("specularMaterial", matComp->getMaterial().specular);
                        loadUniform("shine", matComp->getMaterial().shine);
                    }
                }

                /* Bind normal map */
                auto normalMap = model->getGameObject().getComponentByType<NormalMapComponent>();
                if (normalMap) {
                    normalMap->getTexture().bind();
                    loadUniform("useNormalMap", true);
                    loadUniform("normalMap", normalMap->getTexture().textureId);
                }
                else {
                    loadUniform("useNormalMap", false);
                    loadUniform("N", model->getGameObject().getSpatial()->getNormalMatrix());
                }

                /* DRAW */
                mesh.draw();
            }

            CHECK_GL(glBindVertexArray(0));
            CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
            CHECK_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
            unbind();
    }
};