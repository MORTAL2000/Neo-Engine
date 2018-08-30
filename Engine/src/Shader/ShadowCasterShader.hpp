#pragma once

#include "Shader/Shader.hpp"
#include "GLHelper/Framebuffer.hpp"
#include "System/RenderSystem/RenderSystem.hpp"

#include "NeoEngine.hpp"

namespace neo {

    class ShadowCasterShader : public Shader {

        public:

            ShadowCasterShader(RenderSystem &rSystem) :
                Shader("Shadow Caster",
                    "#version 330 core\n\
                        layout (location = 0) in vec3 vertPos;\
                        layout (location = 2) in vec2 vertTex;\
                        uniform mat4 P, V, M;\
                        out vec2 fragTex;\
                        void main() { gl_Position = P * V * M * vec4(vertPos, 1); fragTex = vertTex; }",
                    "#version 330 core\n\
                        in vec2 fragTex;\
                        uniform bool useTexture;\
                        uniform sampler2D diffuseMap;\
                        void main() {\
                            if (useTexture && texture(diffuseMap, fragTex).a < 0.1) {\
                                discard;\
                            }\
                        }") {
                /* Init shadow map */
                Texture *depthTexture = new Texture2D;
                depthTexture->width = depthTexture->height = 2048;
                depthTexture->components = 1;
                depthTexture->upload(GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_NEAREST, GL_CLAMP_TO_BORDER);
                CHECK_GL(glBindTexture(GL_TEXTURE_2D, depthTexture->textureId));
                CHECK_GL(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, std::vector<float>{1.f, 1.f, 1.f, 1.f}.data()));
                CHECK_GL(glBindTexture(GL_TEXTURE_2D, 0));

                Framebuffer *depthFBO = rSystem.getFBO("depthMap");
                depthFBO->generate();
                depthFBO->attachDepthTexture(*depthTexture);
                depthFBO->disableDraw();
                depthFBO->disableRead();
            }

            virtual void render(const RenderSystem &rSystem, const CameraComponent &camera) override {
                auto fbo = rSystem.framebuffers.find("depthMap")->second.get();
                auto depthTexture = fbo->textures[0];

                fbo->bind();
                CHECK_GL(glClear(GL_DEPTH_BUFFER_BIT));
                CHECK_GL(glViewport(0, 0, depthTexture->width, depthTexture->height));
                CHECK_GL(glCullFace(GL_FRONT));

                bind();

                auto cameras = NeoEngine::getComponents<LightComponent>()[0]->getGameObject().getComponentsByType<CameraComponent>();
                if (cameras.size()) {
                    loadUniform("P", cameras[0]->getProj());
                    loadUniform("V", cameras[0]->getView());
                }

                for (auto model : rSystem.getRenderables<ShadowCasterShader, RenderableComponent>()) {
                    loadUniform("M", model->getGameObject().getSpatial()->getModelMatrix());

                    /* Bind mesh */
                    const Mesh & mesh(model->getMesh());
                    CHECK_GL(glBindVertexArray(mesh.vaoId));
                    CHECK_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.eleBufId));

                    /* Bind texture */
                    auto texComp = model->getGameObject().getComponentByType<TextureComponent>();
                    if (texComp) {
                        auto texture = (Texture2D &) (texComp->getTexture());
                        texture.bind();
                        loadUniform("diffuseMap", texture.textureId);
                        loadUniform("useTexture", true);
                    }
                    else {
                        loadUniform("useTexture", false);
                    }

                    /* Bind texture */

                    /* DRAW */
                    mesh.draw();
                }

                CHECK_GL(glBindVertexArray(0));
                CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
                CHECK_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
                unbind();
                CHECK_GL(glCullFace(GL_BACK));
            }
        };

}