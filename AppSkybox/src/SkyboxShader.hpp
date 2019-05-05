#pragma once

#include "Shader/Shader.hpp"
#include "GLObjects/GLHelper.hpp"
#include "Renderer/Renderer.hpp"

#include "SkyboxComponent.hpp"


class SkyboxShader : public neo::Shader {

    public:

        SkyboxShader(const std::string &vert, const std::string &frag) :
            neo::Shader("Skybox Shader", vert, frag)
        {}

        virtual void render(const neo::CameraComponent &camera) override {
            const auto cube = neo::Engine::getSingleComponent<SkyboxComponent>();
            if (!cube) {
                return;
            }

            CHECK_GL(glDisable(GL_CULL_FACE));
            CHECK_GL(glDepthFunc(GL_LEQUAL));
            bind();

            /* Load PV */
            loadUniform("P", camera.getProj());
            loadUniform("V", camera.getView());

            const neo::Mesh& mesh = *neo::Library::getMesh("cube");
            CHECK_GL(glBindVertexArray(mesh.mVAOID));
            CHECK_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.mElementBufferID));

            /* Bind texture */
            loadUniform("cubeMap", cube->getGameObject().getComponentByType<neo::CubeMapComponent>()->mTexture->mTextureID);

            /* Draw */
            mesh.draw();

            CHECK_GL(glBindVertexArray(0));
            CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));

            unbind();

            CHECK_GL(glEnable(GL_CULL_FACE));
            CHECK_GL(glDepthFunc(GL_LESS));
        }
};