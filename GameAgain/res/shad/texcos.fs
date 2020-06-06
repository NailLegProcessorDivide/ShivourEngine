#version 330 core

out vec4 col;

in vec2 uv;
in vec4 worldNormal;
in vec4 worldPos;

uniform sampler2D myTextureSampler;

uniform vec3 camPos;

uniform vec3 lightPos;
uniform vec3 lightCol;

void main() {
    col = vec4(uv, 0, 0);
}