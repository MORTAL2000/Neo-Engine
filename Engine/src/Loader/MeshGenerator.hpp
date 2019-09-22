
#include "GLObjects/Mesh.hpp"

#include "Util/Util.hpp"

namespace neo {

    class MeshGenerator {

    private:
        static Mesh* _createMesh(const std::vector<float> &vert, const std::vector<float> &norm, const std::vector<float> &tex, const std::vector<unsigned> &ele, unsigned mode = GL_TRIANGLES) {
            Mesh::MeshBuffers buffers{
                vert,
                norm,
                tex,
                ele
            };
            Mesh *mesh = new Mesh(buffers);
            mesh->upload(mode);
            return mesh;
        }

        public:

            static Mesh* createCube() {
                return _createMesh(
                    {-0.5f, -0.5f, -0.5f,
                      0.5f,  0.5f, -0.5f,
                      0.5f, -0.5f, -0.5f,
                     -0.5f,  0.5f, -0.5f,
                     -0.5f, -0.5f, -0.5f,
                     -0.5f,  0.5f,  0.5f,
                     -0.5f,  0.5f, -0.5f,
                     -0.5f, -0.5f,  0.5f,
                     -0.5f,  0.5f, -0.5f,
                      0.5f,  0.5f,  0.5f,
                      0.5f,  0.5f, -0.5f,
                     -0.5f,  0.5f,  0.5f,
                      0.5f, -0.5f, -0.5f,
                      0.5f,  0.5f, -0.5f,
                      0.5f,  0.5f,  0.5f,
                      0.5f, -0.5f,  0.5f,
                     -0.5f, -0.5f, -0.5f,
                      0.5f, -0.5f, -0.5f,
                      0.5f, -0.5f,  0.5f,
                     -0.5f, -0.5f,  0.5f,
                     -0.5f, -0.5f,  0.5f,
                      0.5f, -0.5f,  0.5f,
                      0.5f,  0.5f,  0.5f,
                     -0.5f,  0.5f,  0.5f },
                    { 0,  0, -1,
                      0,  0, -1,
                      0,  0, -1,
                      0,  0, -1,
                     -1,  0,  0,
                     -1,  0,  0,
                     -1,  0,  0,
                     -1,  0,  0,
                      0,  1,  0,
                      0,  1,  0,
                      0,  1,  0,
                      0,  1,  0,
                      1,  0,  0,
                      1,  0,  0,
                      1,  0,  0,
                      1,  0,  0,
                      0, -1,  0,
                      0, -1,  0,
                      0, -1,  0,
                      0, -1,  0,
                      0,  0,  1,
                      0,  0,  1,
                      0,  0,  1,
                      0,  0,  1 },
                    {1.f, 0.f,
                     0.f, 1.f,
                     0.f, 0.f,
                     1.f, 1.f,

                     0.f, 0.f,
                     1.f, 1.f,
                     0.f, 1.f,
                     1.f, 0.f,

                     1.f, 0.f,
                     0.f, 1.f,
                     0.f, 0.f,
                     1.f, 1.f,

                     1.f, 0.f,
                     1.f, 1.f,
                     0.f, 1.f,
                     0.f, 0.f,

                     1.f, 1.f,
                     0.f, 1.f,
                     0.f, 0.f,
                     1.f, 0.f,

                     0.f, 0.f,
                     1.f, 0.f,
                     1.f, 1.f,
                     0.f, 1.f},
                    { 0,  1,  2,
                      0,  3,  1,
                      4,  5,  6,
                      4,  7,  5,
                      8,  9, 10,
                      8, 11,  9,
                     12, 13, 14,
                     12, 14, 15,
                     16, 17, 18,
                     16, 18, 19,
                     20, 21, 22,
                     20, 22, 23 } 
                );
            }

            static Mesh* createQuad() {
                return _createMesh(
                    {-0.5f, -0.5f,  0.f,
                      0.5f, -0.5f,  0.f,
                     -0.5f,  0.5f,  0.f,
                      0.5f,  0.5f,  0.f },
                    {0.f, 0.f, 1.f,
                     0.f, 0.f, 1.f,
                     0.f, 0.f, 1.f,
                     0.f, 0.f, 1.f }, 
                    {0.f, 0.f, 
                     1.f, 0.f,
                     0.f, 1.f,
                     1.f, 1.f }, 
                    {0, 1, 2,
                     1, 3, 2}
                );
            }

            // http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
            static Mesh* createSphere(int recursions) {
	            float t = (float) (1.f + (glm::sqrt(5.0)) / 2.f);
                float length = glm::length(glm::vec3(1, 0, t));
                std::vector<float> verts = {
                     -1.f / length,    t / length,  0.f / length,
                      1.f / length,    t / length,  0.f / length,
                     -1.f / length,   -t / length,  0.f / length,
                      1.f / length,   -t / length,  0.f / length,
                      0.f / length, -1.f / length,    t / length,
                      0.f / length,  1.f / length,    t / length,
                      0.f / length, -1.f / length,   -t / length,
                      0.f / length,  1.f / length,   -t / length,
                        t / length,  0.f / length, -1.f / length,
                        t / length,  0.f / length,  1.f / length,
                       -t / length,  0.f / length, -1.f / length,
                       -t / length,  0.f / length,  1.f / length
                };

                std::vector<unsigned> ele = {
                      0, 11,  5,
                      0,  5,  1,
                      0,  1,  7,
                      0,  7, 10,
                      0, 10, 11,
                      1,  5,  9,
                      5, 11,  4,
                     11, 10,  2,
                     10,  7,  6,
                      7,  1,  8,
                      3,  9,  4,
                      3,  4,  2,
                      3,  2,  6,
                      3,  6,  8,
                      3,  8,  9,
                      4,  9,  5,
                      2,  4, 11,
                      6,  2, 10,
                      8,  6,  7,
                      9,  8,  1,
                };

                for (int i = 1; i <= recursions; i++) {
                    std::vector<unsigned> ele2;
                    for (unsigned j = 0; j <= ele.size() - 3; j += 3) {
                        // find 3 verts of old face
                        glm::vec3 v1(verts[3 * ele[j + 0] + 0], verts[3 * ele[j + 0] + 1], verts[3 * ele[j + 0] + 2]);
                        glm::vec3 v2(verts[3 * ele[j + 1] + 0], verts[3 * ele[j + 1] + 1], verts[3 * ele[j + 1] + 2]);
                        glm::vec3 v3(verts[3 * ele[j + 2] + 0], verts[3 * ele[j + 2] + 1], verts[3 * ele[j + 2] + 2]);

                        // add verts of new tris
                        glm::vec3 halfA = glm::normalize((v1 + v2) / 2.f);
                        glm::vec3 halfB = glm::normalize((v2 + v3) / 2.f);
                        glm::vec3 halfC = glm::normalize((v3 + v1) / 2.f);
                        verts.push_back(halfA.x);
                        verts.push_back(halfA.y);
                        verts.push_back(halfA.z);
                        verts.push_back(halfB.x);
                        verts.push_back(halfB.y);
                        verts.push_back(halfB.z);
                        verts.push_back(halfC.x);
                        verts.push_back(halfC.y);
                        verts.push_back(halfC.z);

                        // add indices of new faces 
                        int indA = verts.size() / 3 - 3;
                        int indB = verts.size() / 3 - 2;
                        int indC = verts.size() / 3 - 1;
                        ele2.push_back(ele[j + 0]);
                        ele2.push_back(indA);
                        ele2.push_back(indC);
                        ele2.push_back(ele[j + 1]);
                        ele2.push_back(indB);
                        ele2.push_back(indA);
                        ele2.push_back(ele[j + 2]);
                        ele2.push_back(indC);
                        ele2.push_back(indB);
                        ele2.push_back(indA);
                        ele2.push_back(indB);
                        ele2.push_back(indC);
                    }

                    ele = ele2;
                }

                // calculate UV coords
                std::vector<float> tex;
                for (unsigned i = 0; i < verts.size(); i += 3) {
                    tex.push_back(glm::clamp(0.5f + std::atan2(verts[i + 2], verts[i]) / (2.f * Util::PI()), 0.f, 1.f));
                    tex.push_back(glm::clamp(0.5f + std::asin(verts[i+1]) / Util::PI(), 0.f, 1.f));
                }

                return _createMesh(
                    verts,
                    verts,
                    tex,
                    ele
                );
            }

            // TODOs
            //  Take height map as input
            //  Move this to its own terrain generator class?
            //  Calculate normals based on verts
            static Mesh* createPlane(float h, int VERTEX_COUNT, int SIZE) {
                int count = VERTEX_COUNT * VERTEX_COUNT;
                std::vector<std::vector<float>> heights;
                std::vector<float> vertices;
                vertices.resize(count * 3);
                std::vector<float> normals;
                normals.resize(count * 3);
                std::vector<float> textureCoords;
                textureCoords.resize(count * 2);
                std::vector<unsigned> indices;
                indices.resize(6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT * 1));
                int vertexPointer = 0;
                for (int i = 0; i < VERTEX_COUNT; i++) {
                    heights.push_back(std::vector<float>());
                    heights[i].resize(VERTEX_COUNT);
                    for (int j = 0; j < VERTEX_COUNT; j++) {
                        vertices[vertexPointer * 3] = (float)j / ((float)VERTEX_COUNT - 1) * SIZE;
                        float height = Util::genRandom(0.f, h); // getHeight(j, i, image);
                        heights[i][j] = height;
                        vertices[vertexPointer * 3 + 1] = height;
                        vertices[vertexPointer * 3 + 2] = (float)i / ((float)VERTEX_COUNT - 1) * SIZE;
                        glm::vec3 normal = glm::vec3(0.f, 1.f, 0.f); // calculateNormal(j, i, image);
                        normals[vertexPointer * 3] = normal.x;
                        normals[vertexPointer * 3 + 1] = normal.y;
                        normals[vertexPointer * 3 + 2] = normal.z;
                        textureCoords[vertexPointer * 2] = (float)j / ((float)VERTEX_COUNT - 1);
                        textureCoords[vertexPointer * 2 + 1] = (float)i / ((float)VERTEX_COUNT - 1);
                        vertexPointer++;
                    }
                }
                int pointer = 0;
                for (int gz = 0; gz < VERTEX_COUNT - 1; gz++) {
                    for (int gx = 0; gx < VERTEX_COUNT - 1; gx++) {
                        int topLeft = (gz*VERTEX_COUNT) + gx;
                        int topRight = topLeft + 1;
                        int bottomLeft = ((gz + 1)*VERTEX_COUNT) + gx;
                        int bottomRight = bottomLeft + 1;
                        indices[pointer++] = topLeft;
                        indices[pointer++] = bottomLeft;
                        indices[pointer++] = topRight;
                        indices[pointer++] = topRight;
                        indices[pointer++] = bottomLeft;
                        indices[pointer++] = bottomRight;
                    }
                }
                return _createMesh(
                    vertices,
                    normals,
                    textureCoords,
                    indices
                );
            }
    };

}
