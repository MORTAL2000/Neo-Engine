#pragma once

#include "Shader/Shader.hpp"
#include "Renderer/Renderer.hpp"
#include "GLObjects/GLHelper.hpp"

namespace neo {

    class WireframeShader : public Shader {

        public:
            WireframeShader() :
                Shader("Wire Shader",
                        "\
                        #version 330 core\n\
                        layout (location = 0) in vec3 vertPos;\
                        uniform mat4 P, V, M;\
                        void main() {\
                            gl_Position = P * V * M * vec4(vertPos, 1);\
                        }",
                        "\
                        #version 330 core\n\
                        out vec4 color;\
                        void main() {\
                            color = vec4(1.0);\
                        }"
                )
            {}

            virtual void render(const CameraComponent &camera) {
                bind();
                CHECK_GL(glDisable(GL_CULL_FACE));

                /* Load PV */
                loadUniform("P", camera.getProj());
                loadUniform("V", camera.getView());

                for (auto& renderable : Engine::getComponents<renderable::WireframeRenderable>()) {
                    auto meshComponent = renderable->getGameObject().getComponentByType<MeshComponent>();
                    if (!meshComponent) {
                        continue; // TODO - assert? 
                    }

                    /* Bind mesh */
                    const Mesh & mesh(meshComponent->getMesh());
                    CHECK_GL(glBindVertexArray(mesh.mVAOID));
                    CHECK_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.mElementBufferID));

                    loadUniform("M", renderable->getGameObject().getSpatial()->getModelMatrix());

                    /* Draw outline */
                    CHECK_GL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
                    mesh.draw();
                    CHECK_GL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
                }

                CHECK_GL(glBindVertexArray(0));
                CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
                unbind();
                CHECK_GL(glEnable(GL_CULL_FACE));
            }
        };

}