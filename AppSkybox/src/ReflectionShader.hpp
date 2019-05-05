#pragma once

#include "Shader/Shader.hpp"
#include "GLObjects/GLHelper.hpp"

#include "Engine.hpp"

#include "ReflectionComponent.hpp"
#include "SkyboxComponent.hpp"


class ReflectionShader : public neo::Shader {

    public:

        ReflectionShader(const std::string &vert, const std::string &frag) :
            neo::Shader("Reflection Shader", vert, frag)
        {}

        virtual void render(const neo::CameraComponent &camera) override {
            bind();

            /* Load PV */
            loadUniform("P", camera.getProj());
            loadUniform("V", camera.getView());
            loadUniform("camPos", camera.getGameObject().getSpatial()->getPosition());

            /* Load environment map */
            if (auto skybox = neo::Engine::getSingleComponent<SkyboxComponent>()) {
                if (auto cubemap = skybox->getGameObject().getComponentByType<neo::CubeMapComponent>()) {
                    loadUniform("cubeMap", cubemap->mTexture->mTextureID);
                }
            }

            for (auto& renderable : neo::Engine::getComponents<ReflectionComponent>()) {
                auto model = renderable->getGameObject().getComponentByType<neo::MeshComponent>();
                if (!model) {
                    continue;
                }

                /* Bind mesh */
                const neo::Mesh & mesh(model->getMesh());
                CHECK_GL(glBindVertexArray(mesh.mVAOID));
                CHECK_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.mElementBufferID));

                loadUniform("M", model->getGameObject().getSpatial()->getModelMatrix());
                loadUniform("N", model->getGameObject().getSpatial()->getNormalMatrix());

                /* DRAW */
                mesh.draw();
            }

            CHECK_GL(glBindVertexArray(0));
            CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
            CHECK_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
            CHECK_GL(glActiveTexture(GL_TEXTURE0));
            CHECK_GL(glBindTexture(GL_TEXTURE_2D, 0));

            unbind();
        }
};