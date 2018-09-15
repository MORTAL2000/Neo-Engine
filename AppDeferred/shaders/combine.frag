#version 330 core

in vec2 fragTex;

uniform sampler2D inputFBO;
uniform sampler2D inputDepth;

uniform sampler2D lightOutput;
uniform sampler2D aoOutput;

out vec4 color;

void main() {
    color = texture(lightOutput, fragTex);
}