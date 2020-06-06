#version 330 core

out vec4 col;

in vec2 uv;
in vec3 normal;

uniform sampler2D myTextureSampler;

void main() {
    col = vec4((normal+vec3(1,1,1))/2, 0);
}