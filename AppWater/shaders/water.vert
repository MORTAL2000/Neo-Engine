
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec4 vertTex;

uniform mat4 P, V, M;

out vec4 fragPos;
out vec4 fragTex;

void main() {
    fragPos = M * vec4(vertPos, 1.0);
    fragTex = vertTex;
    gl_Position = P * V * fragPos;
}