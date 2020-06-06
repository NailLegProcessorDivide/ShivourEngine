#version 330 core

out vec3 col;

in vec2 uv;
in vec3 normal;

uniform sampler2D myTextureSampler;

void main() {
    col = texture( myTextureSampler, uv ).rgb;
}